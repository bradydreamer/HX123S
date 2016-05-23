package com.nexgo.db;

import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteException;
import android.util.Log;

import com.nexgo.common.TradeTlv;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import java.util.HashMap;

/**
 * 交易详情数据库操作
 * Created by xuefeng on 2016/3/1.
 */
public class DbRecordOperation implements Dboperation {

    protected String TAG = "dbRecord";
    private static final String fileName = "/sdcard/xgdRecord.db";
    private static final String backupFileName = "/sdcard/xgdBackup.db";
    private static final String tableName = "TradeDetailInfo";
    private static final String ReversaltableName = "TradeReversalTable";
    private static final String ScripttableName = "ScriptDealResult";
    private static final String OtherInfoTableName = "OtherInfoTable";
    private static final String SaleID = "'trade_id_sale'";
    private static final int NewVersion = 9;//新的版本号
    private static final String vcNo = TradeTlv.VoucherNo;
    private static final String creditDebitFlag = TradeTlv.TradeTyFlag; //借贷计
    private static final String inOutCardFlag = TradeTlv.InOutCardFlag;  //内外卡
    private static final String offlineResult = TradeTlv.OfflineRes;        //脱机交易结果
    private static final String Amount = TradeTlv.Amount;      //金额
    private static boolean beginTransactionflag = false; //事务开启后，不允许关闭数据库，在endTransaction中统一关闭

    private SQLiteDatabase database = null;
    private static DbRecordOperation dbRecordOperation = new DbRecordOperation();

    public static DbRecordOperation getInstance() {
        return dbRecordOperation;
    }

    /**
     * 构造函数 私有 单例模式
     */
    private DbRecordOperation() {
        synchronized (this) {
            //创建数据库
            File file = new File(fileName);
            if (file.exists()) {
//                Log.d(TAG, "打开数据库");
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READWRITE);
                int currentVersion = database.getVersion();
                Log.d(TAG, "当前数据库版本号：" + currentVersion);
                if (currentVersion < NewVersion) {
//                    Log.d(TAG, "升级数据库");
                    beginTransaction();
                    try {
                        onUpgrade(currentVersion, NewVersion);
                        setTransactionSuccessful();
                    } finally {
                        endTransaction();
                    }
                }else {
                    closeRecordDb();
                }
            } else {
//                Log.d(TAG, "创建数据库");
                database = openOrCreateDatabase();
                beginTransaction();
                try {
                    createTables();//创建表，并初始化
                    database.setVersion(NewVersion);
                    setTransactionSuccessful();
                } finally {
                    endTransaction();
                }
            }
        }
    }


    /**
     * 备份数据库
     * 结算前调用，在清流水前将交易记录数据库备份
     * @return
     */
    public long backupDatabase(){
        long nums = -1;
        File dbfile = new File(fileName);
        File backupFile = new File(backupFileName);
        if (dbfile.exists()) {
            try {
                Log.d(TAG, "数据库正在备份中...");
                long timeBegin = System.currentTimeMillis();
                nums = fileCopy(dbfile,backupFile);
                long timeEnd = System.currentTimeMillis();
                Log.d(TAG, "数据库备份花费时间(ms)"+(timeEnd-timeBegin));
            } catch (IOException e) {
                e.printStackTrace();
            }
        }else {
            Log.d(TAG,"数据库未创建！未备份");
        }
        return nums;
    }

    /**
     * 还原数据库
     */
    public long undoDatabase(){
        long nums = -1;
        File dbfile = new File(fileName);
        File backupFile = new File(backupFileName);
        if (backupFile.exists()) {
            try {
                if(dbfile.exists()){
                    dbfile.delete();
                }
                Log.d(TAG,"数据库正在还原中...");
                nums = fileCopy(backupFile,dbfile);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }else {
            Log.d(TAG,"备份数据库丢失！还原失败");
        }
        return nums;
    }

    /**
     * 文件拷贝
     * @param dbFile
     * @param backup
     * @throws IOException
     * @return
     */
    private long fileCopy(File dbFile, File backup) throws IOException {
        long nums = -1;
        FileChannel inChannel = new FileInputStream(dbFile).getChannel();
        FileChannel outChannel = new FileOutputStream(backup).getChannel();
        try {
            nums = inChannel.transferTo(0, inChannel.size(), outChannel);
            Log.d(TAG,"字节数："+nums);
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (inChannel != null) {
                inChannel.close();
            }
            if (outChannel != null) {
                outChannel.close();
            }
        }
        return nums;
    }
    /**
     * 数据库升级，每次修改数据库都要在这里新增版本，同时修改 NewVersion 加1.
     * @param oldVersion
     * @param newVersion
     */
    private void onUpgrade(int oldVersion, int newVersion) {
        synchronized (this) {
            switch (oldVersion) {
                case 0:
                case 1:
                    Log.d(TAG, "从版本1更新到版本2");
                    if(!checkColumnExist(database,tableName,creditDebitFlag)) {//调整交易详情表，新增借贷记标志列
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + creditDebitFlag + " TEXT NOT NULL DEFAULT ''");
                    }
                    if(!checkColumnExist(database,tableName,inOutCardFlag)){//调整交易详情表，新增内外卡标志列
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + inOutCardFlag + " TEXT NOT NULL DEFAULT ''");
                    }
                case 2:
                    Log.d(TAG, "从版本2更新到版本3");
                    database.execSQL(DbRecordTables.ScriptDealResult); //新建脚本结果上送表
                case 3:
                    Log.d(TAG, "从版本3更新到版本4");
                    updateReversalTbale(TradeTlv.ReveralNeed, "0");//向冲正表中默认插入一条冲正标志记录
                case 4:
                    Log.d(TAG, "从版本4更新到版本5");
                    database.execSQL(DbRecordTables.ScriptDealResult);    //添加脚本结果上送表
                    if(!checkColumnExist(database,ScripttableName, TradeTlv.IsHaveScriptReslt)) {//调整脚本表，新增脚本处理结果是否存在的标志列
                        database.execSQL("ALTER TABLE " + ScripttableName + " ADD COLUMN " + TradeTlv.IsHaveScriptReslt + " TEXT NOT NULL DEFAULT ''");
                    }
                case 5:
                    Log.d(TAG, "从版本5更新到版本6");
                    updateScriptTbale(TradeTlv.IsHaveScriptReslt, "0");//向脚本表中默认插入一条脚本标志记录
                case 6:
                    Log.d(TAG, "从版本6更新到版本7");
                    database.execSQL(DbRecordTables.OtherInfoTable); //新建其他表
                    updateOtherInfoTable(TradeTlv.upAdd, "FFFF");//向脚本表中默认插入一条脚本标志记录
                case 7:
                    Log.d(TAG, "从版本7更新到版本8");
                    if(!checkColumnExist(database,tableName,offlineResult)) {//调整交易详情表，新增借贷记标志列
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + offlineResult + " TEXT NOT NULL DEFAULT ''");
                    }
                case 8:
                    Log.d(TAG, "从版本8更新到版本9");
                    if(!checkColumnExist(database,tableName, TradeTlv.InstallmentNum)) {//调整交易详情表，新增分期期数
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + TradeTlv.InstallmentNum + " TEXT NOT NULL DEFAULT ''");
                    }
                    if(!checkColumnExist(database,tableName, TradeTlv.GoodsProjectCode)) {//调整交易详情表，新增商品编码(分期或积分)
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + TradeTlv.GoodsProjectCode + " TEXT NOT NULL DEFAULT ''");
                    }
                    if(!checkColumnExist(database,tableName, TradeTlv.ChargePayMode)) {//调整交易详情表，新增手续费支付方式
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + TradeTlv.ChargePayMode + " TEXT NOT NULL DEFAULT ''");
                    }
                    if(!checkColumnExist(database,tableName, TradeTlv.firstPaymentAmount)) {//调整交易详情表，新增分期首付金额
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + TradeTlv.firstPaymentAmount + " TEXT NOT NULL DEFAULT ''");
                    }
                    if(!checkColumnExist(database,tableName, TradeTlv.InstallmentCharge)) {//调整交易详情表，新增分期手续费
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + TradeTlv.InstallmentCharge + " TEXT NOT NULL DEFAULT ''");
                    }
                    if(!checkColumnExist(database,tableName, TradeTlv.advertisement)) {//调整交易详情表，新增增值广告内容
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + TradeTlv.advertisement + " TEXT NOT NULL DEFAULT ''");
                    }
                    if(!checkColumnExist(database,tableName, TradeTlv.repaymentCurrency)) {//调整交易详情表，新增还款币种
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + TradeTlv.repaymentCurrency + " TEXT NOT NULL DEFAULT ''");
                    }
                    if(!checkColumnExist(database,tableName, TradeTlv.cashPointAmount)) {//调整交易详情表，新增兑换积分数
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + TradeTlv.cashPointAmount + " TEXT NOT NULL DEFAULT ''");
                    }
                    if(!checkColumnExist(database,tableName, TradeTlv.pointBalance)) {//调整交易详情表，新增积分余额
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + TradeTlv.pointBalance + " TEXT NOT NULL DEFAULT ''");
                    }
                    if(!checkColumnExist(database,tableName, TradeTlv.pointPaidAmount)) {//调整交易详情表，新增积分自付金额
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + TradeTlv.pointPaidAmount + " TEXT NOT NULL DEFAULT ''");
                    }
                    if(!checkColumnExist(database,tableName, TradeTlv.transferCardNo)) {//调整交易详情表，新增转入卡卡号
                        database.execSQL("ALTER TABLE " + tableName + " ADD COLUMN " + TradeTlv.transferCardNo + " TEXT NOT NULL DEFAULT ''");
                    }
                case 9:
            }
            database.setVersion(newVersion);
        }
    }

    /**
     * 创建表
     */
    private void createTables() {
        synchronized (this) {
            Log.d(TAG, "创建表");
            database.execSQL(DbRecordTables.TradeDetailInfoTable);//交易详情表
            database.execSQL(DbRecordTables.TradeReversalTable);  //冲正表
            database.execSQL(DbRecordTables.ScriptDealResult);    //脚本结果上送
            updateReversalTbale(TradeTlv.ReveralNeed, "0");//默认插入一条冲正标志记录
            updateScriptTbale(TradeTlv.IsHaveScriptReslt, "0");//默认插入一条脚本标志记录
            database.execSQL(DbRecordTables.OtherInfoTable);    //其他表
            updateOtherInfoTable(TradeTlv.upAdd, "FFFF");
        }
    }
    /**
     * 开启事务
     */
    public void beginTransaction() {
        //如果数据库没打开就打开它
        if (!database.isOpen()) {
            database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READWRITE);
        }
        Log.d(TAG, "开启事务");
        beginTransactionflag = true;
        database.beginTransaction();
    }

    /**
     * 事务已经执行成功
     */
    public void setTransactionSuccessful() {
        //如果数据库没打开就打开它
        if (!database.isOpen()) {
            database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READWRITE);
        }
        Log.d(TAG, "事务执行成功");
        database.setTransactionSuccessful();
    }

    /**
     * 结束事务
     */
    public void endTransaction() {
        //如果数据库没打开就打开它
        if (!database.isOpen()) {
            database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READWRITE);
        }
        Log.d(TAG, "结束事务");
        database.endTransaction();
        beginTransactionflag = false;
        closeRecordDb();//关闭数据库
    }

    /**
     * 打开或创建数据库
     *
     * @return 数据库文件
     */
    public SQLiteDatabase openOrCreateDatabase() {
        synchronized (this) {
            Log.d(TAG, "创建数据库");
            SQLiteDatabase sqLiteDatabase;
            sqLiteDatabase = SQLiteDatabase.openOrCreateDatabase(fileName, null);
            try {
                Runtime.getRuntime().exec("chmod 666 " + fileName);
                Runtime.getRuntime().exec("chmod 666 " + fileName + "-journal");
            } catch (IOException e) {
                e.printStackTrace();
            }
            return sqLiteDatabase;
        }
    }

    /**
     * 打开数据库
     *
     * @param path
     * @param factory
     * @param flags
     * @return
     */
    private SQLiteDatabase openDatabase(String path, SQLiteDatabase.CursorFactory factory, int flags) {
//        Log.d(TAG, "打开数据库");
        return SQLiteDatabase.openDatabase(path, factory, flags);
    }

    /**
     * 检查表中某列是否存在
     * 方法：通过查询sqlite的系统表 sqlite_master 来查找相应表里是否存在该字段，稍微换下语句也可以查找表是否存在
     * @param db
     * @param tableName 表名
     * @param columnName 列名
     * @return
     */
    private boolean checkColumnExist(SQLiteDatabase db, String tableName, String columnName) {
        boolean result = false ;
        Cursor cursor = null ;
        int rows = 0;

        try{
            cursor = db.rawQuery("select * from sqlite_master where name = ? and sql like ?", new String[]{tableName, "%" + columnName + "%"});
            rows = cursor.getCount();
            if(rows == 1){
                Log.d(TAG,"表中存在列(标签)："+columnName);
                result = true;//true:表中存在查询的列
            }else if(rows == 0){
                Log.d(TAG,"表中没有该列(标签)："+columnName);
            }
        }catch (Exception e){
            Log.e(TAG,"checkColumnExist..." + e.getMessage()) ;
        }finally{
            if(null != cursor && !cursor.isClosed()){
                cursor.close() ;
            }
        }
        return result ;
    }

    @Override
    /**
     * 向交易详情表中插入或者批量更新一条交易记录，多标签更新
     * @param cv 标签对 <标签名,值>
     * @param VoucherNO 流水号
     * @return 执行状态 false-成功，true-失败
     */
    public boolean update(ContentValues cv, String VoucherNO) {
        synchronized (this) {
            Log.d(TAG, "批量更新记录表");
            String whereClause;
            String[] whereArgs;

            if (cv == null || VoucherNO == null) {
                Log.e(TAG, "请输入正确的参数");
                return false;
            }

            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READWRITE);
            }

            boolean ext = false;
            Cursor cursor = database.query(tableName, null, vcNo + " = ?", new String[]{VoucherNO}, null, null, null);
            int count = cursor.getCount();
            cursor.close();

            if (count == 1) {
                ext = true;
            }else if(count>1){
                Log.e(TAG, "数据库异常;存在同一流水号的多条数据");
                closeRecordDb();
                return false;
            }
            //如果流水号存在就更新数据，否则插入数据
            if (!ext) {
                if (!cv.containsKey(vcNo)) {
                    cv.put(vcNo, VoucherNO);
                }
                try {
                    Log.d(TAG, "插入一条记录");
                    database.insert(tableName, null, cv);
                } catch (SQLiteException sqle) {
                    sqle.printStackTrace();
                    closeRecordDb();
                    return false;
                }
            } else {
                whereClause = vcNo + " = ?";
                whereArgs = new String[]{VoucherNO};
                //更新数据
                try {
                    count = database.update(tableName, cv, whereClause, whereArgs);
//                    Log.e(TAG, String.valueOf(count));
                } catch (SQLiteException sqlE) {
                    sqlE.printStackTrace();
                    closeRecordDb();
                    return false;
                }
            }

            closeRecordDb();
            return true;
        }
    }

    @Override
    /**
     * 向交易详情表中插入或者更新一条交易记录，单标签更新
     * @param labalName 标签名
     * @param value 标签对应的值
     * @param VoucherNO 流水号
     * @return
     */
    public boolean update(String labalName, String value, String VoucherNO) {
        synchronized (this) {
            Log.d(TAG, "更新交易记录表");
            String whereClause = vcNo + " = ?";
            String[] whereArgs = new String[]{VoucherNO};
            if (labalName == null || labalName.equals("PID") || VoucherNO == null) {
                Log.e(TAG, "请输入正确的参数");
                return false;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READWRITE);
            }

            ContentValues cv = new ContentValues();
            cv.put(labalName, value);
            Cursor cursor = database.query(tableName, null, whereClause, whereArgs, null, null, null);
            int count = cursor.getCount();
            cursor.close();
            if (count == 1) {
                //更新数据
                try {
                    count = database.update(tableName, cv, whereClause, whereArgs);
                    Log.d(TAG, "更新 "+String.valueOf(count)+" 个标签");
                } catch (SQLiteException sqlE) {
                    return false;
                }
            }else if(count>1){
                Log.e(TAG, "数据库异常;存在同一流水号的多条数据");
                closeRecordDb();
                return false;
            }else {
                Log.d(TAG, "插入一条记录");
                if (!cv.containsKey(vcNo)) {
                    cv.put(vcNo, VoucherNO);
                }
                database.insert(tableName, null, cv);
            }
            closeRecordDb();
            return true;
        }
    }

    @Override
    /**
     * 批量获取标签值
     * 在交易详情表中，根据流水号找到对应记录，取出传入的标签的值，以标签对的方式输出
     * @param labels 标签名，以字符串数组方式传入
     * @param VoucherNO 流水号
     * @return 以标签对的方式输出 <label,value>
     */
    public ContentValues getData(String[] labels, String VoucherNO) {
        synchronized (this) {
            Log.d(TAG, "批量获取记录表数据");
            String value;
            String selection = vcNo + " = ?";
            String[] selectionArgs = new String[]{VoucherNO};
            ContentValues cv = new ContentValues();

            if (labels == null || VoucherNO == null) {
                Log.e(TAG, "请输入正确的参数");
                return null;
            }

            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }
            Cursor cursor = database.query(tableName, null, selection, selectionArgs, null, null, null);
            int count = cursor.getCount();
            if (count != 1) {
                if(count>1) {
                    Log.e(TAG, "数据库异常;存在同一流水号的多条数据");
                }else if(count==0){
                    Log.e(TAG, "没有该流水记录");
                }
                cursor.close();
                closeRecordDb();
                return null;
            }
            cursor.moveToFirst();
            int index;
            for (String label : labels) {
                index = cursor.getColumnIndex(label);
                if (index == -1) {
                    Log.e(TAG,"交易详情表中没有该标签："+label+"批量获取数据失败");
                    cursor.close();
                    closeRecordDb();
                    return null;
                } else {
                    value = cursor.getString(index);
                }
                cv.put(label, value);
            }
            cursor.close();
            closeRecordDb();
            return cv;
        }
    }

    @Override
    /**
     * 获取单个标签的值
     * 在交易详情表中，根据流水号找到对应记录，取出传入的标签的值，以字符串方式输出
     * @param labalName 参数的标签
     * @param VoucherNO
     * @return 返回标签对应的value
     */
    public String getData(String labelName, String VoucherNO) {
        synchronized (this) {
            Log.d(TAG, "获取记录表数据");
            String value;
            String selection = vcNo + " = ?";
            String[] selectionArgs = new String[]{VoucherNO};

            if (labelName == null || VoucherNO == null) {
                Log.e(TAG, "请输入正确的参数");
                return null;
            }

            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }
            Cursor cursor = database.query(tableName, null, selection, selectionArgs, null, null, null);
            int count = cursor.getCount();
            if (count != 1) {
                if(count>1) {
                    Log.e(TAG, "数据库异常;存在同一流水号的多条数据");
                }else if(count==0){
                    Log.e(TAG, "没有该流水记录");
                }
                cursor.close();
                closeRecordDb();
                return null;
            }
            cursor.moveToFirst();
            int index = cursor.getColumnIndex(labelName);
            if (index == -1) {
                Log.e(TAG,"交易详情表中没有该标签："+labelName+"获取数据失败");
                cursor.close();
                closeRecordDb();
                return null;
            } else {
                value = cursor.getString(index);
            }

            cursor.close();
            closeRecordDb();
            return value;
        }
    }

    @Override
    /**
     * 根据流水号返回一条交易记录,包含所有标签
     * @param VoucherNO
     * @return 标签对，如果为空，表示没有对应流水号的记录
     * */
    public ContentValues getOneRecord(String VoucherNO) {
        synchronized (this) {
            Log.d(TAG, "获取记录表中一条数据");
            if (VoucherNO == null) {
                Log.e(TAG, "请输入正确的参数");
                return null;
            }

            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }

            ContentValues cv = new ContentValues();
            Cursor cursor = database.query(tableName, null, vcNo + "=?", new String[]{VoucherNO}, null, null, null);
            int count = cursor.getCount();
            if (count != 1) {
                if(count>1) {
                    Log.e(TAG, "数据库异常;存在同一流水号的多条数据");
                }else if(count==0){
                    Log.e(TAG, "没有该流水记录");
                }
                cursor.close();
                closeRecordDb();
                return null;
            }
            cursor.moveToFirst();
            for (count = 0; count < cursor.getColumnCount(); count++) {
                String name = cursor.getColumnName(count);
                String value = cursor.getString(count);
                cv.put(name, value);
//            Log.d(TAG,"query------->"+"name：" + name + " " + "value：" + value);
            }
            cursor.close();
            closeRecordDb();
            return cv;
        }
    }

    /**
     * 根据索引号获取一条交易记录
     * @param index 偏移的索引号 0~getrecordNums()-1
     * @return
     */
    public ContentValues getOneRecordByIndex(int index){
        synchronized (this) {
            if (index <0 ) {
                Log.e(TAG, "索引号不能小于0");
                return null;
            }
            Log.d(TAG, "获取记录表中 第"+index+" 条记录");
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }
            ContentValues cv = new ContentValues();
            String limit = " LIMIT " + Integer.toString(1) + " OFFSET " + Integer.toString(index);//跳过index行取1行
            Cursor cursor = database.rawQuery("SELECT * FROM " + tableName + limit, null);
            int count = cursor.getCount();
            if(count==0){
                Log.e(TAG, "没有该流水记录");
                cursor.close();
                closeRecordDb();
                return null;
            }
            cursor.moveToFirst();
            for (count = 0; count < cursor.getColumnCount(); count++) {
                String name = cursor.getColumnName(count);
                String value = cursor.getString(count);
                cv.put(name, value);
//            Log.d(TAG,"query------->"+"name：" + name + " " + "value：" + value);
            }
            cursor.close();
            closeRecordDb();
            return cv;
        }
    }

    /**
     * 从数据库中查询最近的消费交易明细，用于撤销和重打印显示。
     * 筛选条件：消费交易；
     * 返回成功的消费交易记录
     * @return 除了报文不返回，其余信息都返回。一次最多返回20条记录
     */
    public synchronized ArrayList<ContentValues> getSucessConsumeInfo(int index) {
        ArrayList<ContentValues> list = new ArrayList<>();
        int rowIndex;
        int columnIndex;

        //如果数据库没打开就打开它
        if (!database.isOpen()) {
            database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
        }
        String orderBy = " PID DESC";//主键降序排列（从最近的交易开始显示）
        String limit = " LIMIT " + Integer.toString(20) + " OFFSET " + Integer.toString(index);//跳过index行取20行
        Cursor cursor = database.rawQuery("SELECT * FROM " + tableName + " WHERE " + TradeTlv.TradeId +
                " = " + SaleID + " ORDER BY " + orderBy + limit, null);
        int totalRows = cursor.getCount();//记录数
        if(totalRows==0){
            Log.e(TAG, "没有成功的消费记录");
            cursor.close();
            closeRecordDb();
            return null;
        }
        cursor.moveToFirst();
        for (rowIndex = 0; rowIndex < totalRows; rowIndex++) {
            ContentValues cv = new ContentValues();
            for (columnIndex = 0; columnIndex < cursor.getColumnCount(); columnIndex++) {
                String name = cursor.getColumnName(columnIndex);
                String value = cursor.getString(columnIndex);
                if (name.equals(TradeTlv.SendMessage) ||
                        name.equals(TradeTlv.RecvMessage) ||
                        name.equals(TradeTlv.Filed55) ||
                        name.equals(TradeTlv.IsUpLoad)) {
                    continue;
                } else {
                    cv.put(name, value);
                }
            }
            list.add(cv);
            Log.d(TAG, "query------->" + "name：" + TradeTlv.VoucherNo + "value：" + list.get(rowIndex).get(TradeTlv.VoucherNo));
            cursor.moveToNext();
        }

        cursor.close();
        closeRecordDb();
        return list;
    }

    /**
     * 从数据库中查询最近的所有交易明细
     * @param index
     * @return
     */
    public synchronized ArrayList<ContentValues> getAllInfo(int index){
        ArrayList<ContentValues> list = new ArrayList<>();
        int rowIndex;
        int columnIndex;

        //如果数据库没打开就打开它
        if (!database.isOpen()) {
            database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
        }
        String orderBy = " PID DESC";//主键降序排列（从最近的交易开始显示）
        String limit = " LIMIT " + Integer.toString(20) + " OFFSET " + Integer.toString(index);//跳过index行取20行
        Cursor cursor = database.rawQuery("SELECT * FROM " + tableName
//                + " WHERE " + TradeTlv.TradeId + " = " + SaleID
                + " ORDER BY " + orderBy + limit, null);
        int totalRows = cursor.getCount();//记录数
        if(totalRows==0){
            Log.e(TAG, "没有流水记录");
            cursor.close();
            closeRecordDb();
            return null;
        }
        cursor.moveToFirst();
        for (rowIndex = 0; rowIndex < totalRows; rowIndex++) {
            ContentValues cv = new ContentValues();
            for (columnIndex = 0; columnIndex < cursor.getColumnCount(); columnIndex++) {
                String name = cursor.getColumnName(columnIndex);
                String value = cursor.getString(columnIndex);
                if (name.equals(TradeTlv.SendMessage) ||
                        name.equals(TradeTlv.RecvMessage) ||
                        name.equals(TradeTlv.Filed55)) {
                    continue;
                } else {
                    cv.put(name, value);
                }
            }
            list.add(cv);
            Log.d(TAG, "query------->" + "name：" + TradeTlv.VoucherNo + "value：" + list.get(rowIndex).get(TradeTlv.VoucherNo));
            cursor.moveToNext();
        }

        cursor.close();
        closeRecordDb();
        return list;
    }

    @Override
    /**
     * 删除交易详情表中的所有记录
     * @return 删除的记录数目
     */
    public int deleteAll() {
        synchronized (this) {
            Log.d(TAG, "删除记录表所有记录");
            long counts = backupDatabase();//清空流水前先备份整个数据库
            if(counts==-1){
                Log.d(TAG, "备份数据库失败，已取消删除交易流水操作！");
                return -1;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READWRITE);
            }
            int nums = database.delete(tableName, null, null);
            if(nums>=0) {
                Log.d(TAG, "成功删除 " + nums + " 条交易流水记录");
            }
            closeRecordDb();
            return nums;
        }
    }

    @Override
    /**
     * 根据输入流水号删除交易详情表的对应的交易记录
     * @param VoucherNO 流水号
     * @return
     */
    public boolean delete(String VoucherNO) {
        synchronized (this) {
            Log.d(TAG, "更删除记录表一条记录");
            String selection = vcNo + " = ?";
            String[] selectionArgs = new String[]{VoucherNO};

            if (VoucherNO == null) {
                Log.e(TAG, "请输入正确的参数");
                return false;
            }

            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READWRITE);
            }
            Cursor cursor = database.query(tableName, null, selection, selectionArgs, null, null, null);
            int count = cursor.getCount();
            cursor.close();
            if (count == 1) {
                Log.d(TAG,"成功删除流水号为："+VoucherNO+"的记录");
                database.delete(tableName, selection, selectionArgs);
                closeRecordDb();
                return true;
            }
            else{
                if(count == 0) {
                    Log.e(TAG, "删除记录失败，不存在流水号为："+VoucherNO+"的记录");
                }else if(count>1){
                    Log.e(TAG, "交易详情表中存在重复的流水号的交易记录");
                }
                closeRecordDb();
                return false;
            }
        }
    }

    @Override
    /**
     * 统计交易详情表中的总记录条数
     * @return 记录数目
     */
    public int getrecordNums() {
        synchronized (this) {
            Log.d(TAG, "获取记录表总条数");
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }
            // 调用查找并返回数据源
            Cursor cursor = database.rawQuery("select * from " + tableName, null);
            int count = cursor.getCount();

            cursor.close();
            closeRecordDb();
            return count;
        }
    }

    /**
     * 关闭终端参数数据库文件
     */
    private void closeRecordDb() {
        synchronized (this) {
            //关闭当前数据库
            if (database.isOpen()) {
                if (!beginTransactionflag) {
//                    Log.d(TAG, "关闭数据库");
                    database.close();
                }
            }
        }
    }

    /**
     * @param VoucherNO
     * @return
     */
    @Override
    /**
     * 判断交易详情表中是否存在输入的流水号的记录
     * @param VoucherNO 流水号
     * @return 记录数目
     * @throws Exception
     */
    public int isVoucherNOExist(String VoucherNO) {
        synchronized (this) {
            Log.d(TAG, "查询记录表有无对应流水存在");
            String selection = vcNo + " = ?";
            String[] selectionArgs = new String[]{VoucherNO};

            if (VoucherNO == null) {
                Log.e(TAG, "请输入正确的参数");
                return -1;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }
            Cursor cursor = database.query(tableName, null, selection, selectionArgs, null, null, null);
            int count = cursor.getCount();
            cursor.close();
            closeRecordDb();
            return count;
        }
    }

    /**
     * 按交易类型统计笔数和金额
     * @param tradeId 交易ID
     * @return
     */
    public HashMap settleResult(String tradeId){
        synchronized (this){
            Log.d(TAG,"按交易类型统计笔数和金额");
            HashMap map = new HashMap();
            int count = 0;
            long totalAmount = 0;
            Cursor cursor = null;
            String selection = TradeTlv.TradeId + " = ? ";
            String[] selectionArgs = new String[]{tradeId};
            String[] selectionColumn = new String[]{Amount};
            if (tradeId == null) {
                Log.e(TAG, "请输入正确的参数");
                return null;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }
            cursor = database.query(tableName, selectionColumn, selection, selectionArgs, null, null, null);
            count = cursor.getCount();
            if (count == 0) {
                Log.e(TAG, "没有符合条件的交易记录");
                cursor.close();
                closeRecordDb();
                return null;
            }
            cursor.moveToFirst();
            int columnIndex = cursor.getColumnIndex(Amount);
            do {
                totalAmount += Long.parseLong(cursor.getString(columnIndex));
            } while (cursor.moveToNext());
//        Log.d(TAG,"totalAmount = "+totalAmount);
            cursor.close();
            closeRecordDb();
            map.put("Amount", totalAmount);
            map.put("Count", count);
            return map;
        }
    }


    /**
     * 按交易类型统计笔数和金额
     * @param tradeId 交易ID
     * @param cardFlag 内外卡 0-内卡 1-外卡
     * @return
     */
    public HashMap settleStatistics(String tradeId,String cardFlag){
        synchronized (this){
            Log.d(TAG,"按交易类型和内外卡统计笔数和金额");
            HashMap map = new HashMap();
            int count = 0;
            long totalAmount = 0;
            Cursor cursor = null;
            String selection = TradeTlv.TradeId + " = ? " + " and " + inOutCardFlag + " = ? ";
            String[] selectionArgs = new String[]{tradeId,cardFlag};
            String[] selectionColumn = new String[]{Amount};
            if (tradeId == null) {
                Log.e(TAG, "请输入正确的参数");
                return null;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }
            cursor = database.query(tableName, selectionColumn, selection, selectionArgs, null, null, null);
            count = cursor.getCount();
            if (count == 0) {
                Log.e(TAG, "没有符合条件的交易记录");
                cursor.close();
                closeRecordDb();
                return null;
            }
            cursor.moveToFirst();
            int columnIndex = cursor.getColumnIndex(Amount);
            do {
                totalAmount += Long.parseLong(cursor.getString(columnIndex));
            } while (cursor.moveToNext());
//        Log.d(TAG,"totalAmount = "+totalAmount);
            cursor.close();
            closeRecordDb();
            map.put("Amount", totalAmount);
            map.put("Count", count);
            return map;
        }
    }
    /**
     * 按交易类型统计笔数和金额
     * @param tradeId 交易ID
     * @param uploadstate     0-未上送 1-上送成功 2-失败 3-上送被拒
     * @return
     */
    public HashMap settleStatistics(String tradeId,int uploadstate){
        synchronized (this){
            Log.d(TAG,"按交易类型和内外卡统计笔数和金额");
            HashMap map = new HashMap();
            int count = 0;
            long totalAmount = 0;
            Cursor cursor = null;
            String selection = TradeTlv.TradeId + " = ? " + " and " + TradeTlv.IsUpLoad + " = ? ";
            String strstate=uploadstate+"";
            String[] selectionArgs = new String[]{tradeId,strstate};
            String[] selectionColumn = new String[]{Amount};
            if (tradeId == null) {
                Log.e(TAG, "请输入正确的参数");
                return null;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }
            cursor = database.query(tableName, selectionColumn, selection, selectionArgs, null, null, null);
            count = cursor.getCount();
            if (count == 0) {
                Log.e(TAG, "没有符合条件的交易记录");
                cursor.close();
                closeRecordDb();
                return null;
            }
            cursor.moveToFirst();
            int columnIndex = cursor.getColumnIndex(Amount);
            do {
                totalAmount += Long.parseLong(cursor.getString(columnIndex));
            } while (cursor.moveToNext());
//        Log.d(TAG,"totalAmount = "+totalAmount);
            cursor.close();
            closeRecordDb();
            map.put("Amount", totalAmount);
            map.put("Count", count);
            return map;
        }
    }

    /**
     * 根据类型统计结算金额和笔数
     * @param tradeFlag 借贷记 0-借记 1-贷记
     * @param cardFlag  内外卡 0-内卡 1-外卡
     * @return HashMap 存放金额和笔数
     */
    public HashMap settleResult(String tradeFlag, String cardFlag) {
        synchronized (this) {
            Log.d(TAG, "记录表结算统计");
            HashMap map = new HashMap();
            int count = 0;
            long totalAmount = 0;
            Cursor cursor = null;
            String selection = creditDebitFlag + " = ? " + " and " + inOutCardFlag + " = ? ";
            String[] selectionArgs = new String[]{tradeFlag, cardFlag};
            String[] selectionColumn = new String[]{Amount};
            if (tradeFlag == null || cardFlag == null) {
                Log.e(TAG, "请输入正确的参数");
                return null;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }
            cursor = database.query(tableName, selectionColumn, selection, selectionArgs, null, null, null);
            count = cursor.getCount();
            if (count == 0) {
                Log.e(TAG, "没有符合条件的交易记录");
                cursor.close();
                closeRecordDb();
                return null;
            }
            cursor.moveToFirst();
            int columnIndex = cursor.getColumnIndex(Amount);
            do {
                totalAmount += Long.parseLong(cursor.getString(columnIndex));
            } while (cursor.moveToNext());
//        Log.d(TAG,"totalAmount = "+totalAmount);
            cursor.close();
            closeRecordDb();
            map.put("Amount", totalAmount);
            map.put("Count", count);
            return map;
        }
    }

    @Override
    /**
     * 向冲正表中插入或者更新一条交易记录，单标签更新
     * @param labalName 标签名
     * @param value 标签对应的值
     * @return
     */
    public boolean updateReversalTbale(String labalName, String value) {
        synchronized (this) {
            Log.d(TAG, "更新冲正表");
            ContentValues cv = new ContentValues();

            if (labalName == null || labalName.equals("PID")) {
                Log.e(TAG, "请输入正确的参数");
                return false;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READWRITE);
            }

            cv.put(labalName, value);

            //更新数据
            Cursor cursor = database.rawQuery("select * from " + ReversaltableName, null);
            int count = cursor.getCount();
            cursor.close();
            if (count == 0) {
                Log.d(TAG,"插入一条冲正记录");
                database.insert(ReversaltableName, null, cv);
            } else if(count == 1){
                try {
                    Log.d(TAG,"更新冲正记录");
                    database.update(ReversaltableName, cv, null, null);
                } catch (SQLiteException sqlE) {
                    closeRecordDb();
                    return false;
                }
            } else{
                Log.e(TAG, "数据库异常，冲正表存在多条冲正记录");
                closeRecordDb();
                return false;
            }
            closeRecordDb();
            return true;
        }
    }

    @Override
    /**
     * 获取冲正表中的单个标签的值
     * @param labalName 参数的标签
     * @return 返回标签对应的value
     */
    public String getReversalData(String labalName) {
        synchronized (this) {
            Log.d(TAG, "获取冲正表数据");
            String value;

            if (labalName == null) {
                Log.e(TAG, "请输入正确的参数");
                return null;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }
            Cursor cursor = database.query(ReversaltableName, null, null, null, null, null, null);
            int count = cursor.getCount();
            if(count!=1){
                if(count==0){
                    Log.e(TAG, "没有冲正记录");
                }else if(count>1) {
                    Log.e(TAG, "数据库异常，冲正表存在多条冲正记录");
                }
                cursor.close();
                closeRecordDb();
                return null;
            }
            cursor.moveToFirst();
            int index = cursor.getColumnIndex(labalName);
            if (index == -1) {
                Log.e(TAG, "数据库异常;不存在该标签："+labalName);
                cursor.close();
                closeRecordDb();
                return null;
            } else {
                value = cursor.getString(index);
            }
            cursor.close();
            closeRecordDb();
            return value;
        }
    }

    @Override
    /**
     * 向脚本上送结果表中插入或者更新一条交易记录，单标签更新
     * @param labalName 标签名
     * @param value 标签对应的值
     * @return
     */
    public boolean updateScriptTbale(String labalName, String value) {
        synchronized (this) {
            Log.d(TAG, "更新脚本表数据");
            ContentValues cv = new ContentValues();

            if (labalName == null || labalName.equals("PID")) {
                Log.e(TAG, "请输入正确的参数");
                return false;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READWRITE);
            }
            cv.put(labalName, value);
            //更新数据
            Cursor cursor = database.rawQuery("select * from " + ScripttableName, null);
            int count = cursor.getCount();
            cursor.close();
            if (count == 0) {
                Log.d(TAG,"插入一条脚本记录,标签名："+labalName);
                database.insert(ScripttableName, null, cv);
            } else if(count == 1){
                try {
                    Log.d(TAG,"更新脚本记录,标签名："+labalName);
                    database.update(ScripttableName, cv, null, null);
                } catch (SQLiteException sqlE) {
                    closeRecordDb();
                    return false;
                }
            } else{
                Log.e(TAG, "数据库异常;脚本表存在多条脚本记录");
                closeRecordDb();
                return false;
            }
            closeRecordDb();
            return true;
        }
    }

    @Override
    /**
     * 获取脚本上送结果表中的单个标签的值
     * @param labalName 参数的标签
     * @return 返回标签对应的value
     */
    public String getScriptData(String labalName) {
        synchronized (this) {
            Log.d(TAG, "获取脚本表数据");
            String value;

            if (labalName == null) {
                Log.e(TAG, "请输入正确的参数");
                return null;
            }

            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }
            Cursor cursor = database.query(ScripttableName, null, null, null, null, null, null);
            int count = cursor.getCount();
            if(count!=1){
                if(count==0){
                    Log.e(TAG, "没有脚本记录");
                }else if(count>1) {
                    Log.e(TAG, "数据库异常;存在多条脚本记录");
                }
                cursor.close();
                closeRecordDb();
                return null;
            }
            cursor.moveToFirst();
            int index = cursor.getColumnIndex(labalName);
            if (index == -1) {
                Log.e(TAG, "数据库异常;不存在该标签："+labalName);
                cursor.close();
                closeRecordDb();
                return null;
            } else {
                value = cursor.getString(index);
            }
            cursor.close();
            closeRecordDb();
            return value;
        }
    }

    @Override
    /**
     * 向其他表中插入或者更新一条记录，单标签更新
     * @param labalName 标签名
     * @param value 标签对应的值
     * @return
     */
    public boolean updateOtherInfoTable(String labalName, String value) {
        synchronized (this) {
            Log.d(TAG, "更新其他表数据");
            ContentValues cv = new ContentValues();

            if (labalName == null || labalName.equals("PID")) {
                Log.e(TAG, "请输入正确的参数");
                return false;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READWRITE);
            }
            cv.put(labalName, value);
            //更新数据
            Cursor cursor = database.rawQuery("select * from " + OtherInfoTableName, null);
            int count = cursor.getCount();
            cursor.close();
            if (count == 0) {
                Log.d(TAG,"向其他表插入一条记录,标签名："+labalName);
                database.insert(OtherInfoTableName, null, cv);
            } else if(count == 1){
                try {
                    Log.d(TAG,"更新其他表记录,标签名："+labalName);
                    database.update(OtherInfoTableName, cv, null, null);
                } catch (SQLiteException sqlE) {
                    closeRecordDb();
                    return false;
                }
            } else{
                Log.e(TAG, "数据库异常;其他表存在多条记录");
                closeRecordDb();
                return false;
            }
            closeRecordDb();
            return true;
        }
    }

    @Override
    /**
     * 获取其他表中的单个标签的值
     * @param labalName 参数的标签
     * @return 返回标签对应的value
     */
    public String getOtherInfoTableData(String labalName) {
        synchronized (this) {
            Log.d(TAG, "获取其他表数据");
            String value;

            if (labalName == null) {
                Log.e(TAG, "请输入正确的参数");
                return null;
            }

            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openDatabase(fileName, null, SQLiteDatabase.OPEN_READONLY);
            }
            Cursor cursor = database.query(OtherInfoTableName, null, null, null, null, null, null);
            int count = cursor.getCount();
            if(count!=1){
                if(count==0){
                    Log.e(TAG, "没有记录");
                }else if(count>1) {
                    Log.e(TAG, "数据库异常;存在多条记录");
                }
                cursor.close();
                closeRecordDb();
                return null;
            }
            cursor.moveToFirst();
            int index = cursor.getColumnIndex(labalName);
            if (index == -1) {
                Log.e(TAG, "数据库异常;不存在该标签："+labalName);
                cursor.close();
                closeRecordDb();
                return null;
            } else {
                value = cursor.getString(index);
            }
            cursor.close();
            closeRecordDb();
            return value;
        }
    }
}
