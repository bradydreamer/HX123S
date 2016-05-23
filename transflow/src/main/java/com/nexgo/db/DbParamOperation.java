package com.nexgo.db;

import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

import java.io.IOException;

/**
 * Created by xuefeng on 2016/2/29.
 */
public class DbParamOperation {

    private SQLiteDatabase database = null;
    protected String TAG = "db";
    //private final String fileName="/private/config/xgdParam.db";
    private final String fileName = "/private/config/xgdParam.db";

    // private final String fileName="/data/data/com.nexgo.ump/xgdParam.db";
    private static DbParamOperation dbParamOperation = new DbParamOperation();

    String[] stables = {DbParamTables.CardTypeTotalInfoTable, DbParamTables.TradeTotalInfoTable,
            DbParamTables.termParamTable, DbParamTables.commParamTable, DbParamTables.OperatorTable};
    String[] tablesName = {"CardTypeTotalInfo", "TradeTotalInfo", "TerminalParam", "CommunicateParam", "OperatorParam"};

    public static DbParamOperation getInstance() {
        return dbParamOperation;
    }

    public DbParamOperation() {
        Log.i(TAG, "创建或打开文件！");
        //创建数据库
        database = openOrCreateDatabase();
        //创建表，并初始化
        createTables();

    }

    private void createTables() {
        for (int index = 0; index < stables.length; index++) {
            database.execSQL(stables[index]);
            Cursor cursor = database.query(tablesName[index], null, null, null, null, null, null);
            int count = cursor.getCount();
            //如果没有数据就插入一条，有一条数据就下次循环，其它情况下数据库错误
            if (count == 0) {
                ContentValues values = new ContentValues();
                values.put("pid", 0);
                database.insert(tablesName[index], null, values);
            } else if (count == 1) {
                continue;
            } else {
                Log.e(TAG, "数据库错误");
            }
        }
    }

    public SQLiteDatabase openOrCreateDatabase() {
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

    public int update(String tableName, ContentValues cv) {
        //如果数据库没打开就打开它
        if (!database.isOpen()) {
            database = openOrCreateDatabase();
        }

        //更新数据
        int count = database.update(tableName, cv, null, null);
        Log.e(TAG, "更新记录条数" + String.valueOf(count));
        return count;
    }

    /**
     * 获取终端参数中的通讯参数，统一以String类型返回，涉及到bool类型的true以“1”,false以
     * “0”返回。无值返回 null
     *
     * @param tableName 数据库表
     * @param labels    参数的系列标签
     * @return 返回标签对应的value
     */
    public ContentValues getData(String tableName, String... labels) {

        String value = null;
        //如果数据库没打开就打开它
        if (!database.isOpen()) {
            database = openOrCreateDatabase();
        }

        Cursor cursor = database.query(tableName, null, null, null, null, null, null);

        if (cursor.getCount() != 1) {
            Log.e(TAG, "数据库异常");
            return null;
        }
        cursor.moveToFirst();
        ContentValues cv = new ContentValues();
        for (String label : labels) {
            value = cursor.getString(cursor.getColumnIndex(label));
            cv.put(label, value);
        }
        return cv;
    }

    public boolean update(String tableName, String labalName, String value) {
        //如果数据库没打开就打开它
        if (!database.isOpen()) {
            database = openOrCreateDatabase();
        }

        ContentValues cv = new ContentValues();
        cv.put(labalName, value);

        //更新数据
        int count = database.update(tableName, cv, null, null);
        Log.e(TAG, String.valueOf(count));
        return true;
    }

    /**
     * 获取终端参数中的通讯参数，统一以String类型返回，涉及到bool类型的true以“1”,false以
     * “0”返回。无值返回 null
     *
     * @param tableName 数据库表
     * @param labalName 参数的标签
     * @return 返回标签对应的value
     */
    public String getData(String tableName, String labalName) {

        String value = null;
        //如果数据库没打开就打开它
        if (!database.isOpen()) {
            database = openOrCreateDatabase();
        }

        Cursor cursor = database.query(tableName, null, null, null, null, null, null);

        if (cursor.getCount() != 1) {
            Log.e(TAG, "数据库异常");
            return null;
        }
        cursor.moveToFirst();
        value = cursor.getString(cursor.getColumnIndex(labalName));
        return value;
    }


    /**
     * 获取终端参数数据库文件内对应数据的数据类型
     * Returned column types are
     * <p>
     * FIELD_TYPE_NULL
     * FIELD_TYPE_INTEGER
     * FIELD_TYPE_FLOAT
     * FIELD_TYPE_STRING
     * FIELD_TYPE_BLOB
     *
     * @param labalName 参数的标签
     * @return 返回标签对应的value的在数据库文件存储的数据类型
     */
    public int getParamDataType(String tableName, String labalName) {

        String tableSql;

        //如果数据库没打开就打开它
        if (!database.isOpen()) {
            database = openOrCreateDatabase();
        }

        tableSql = "SELECT * FROM " + tableName;
        Cursor c = database.rawQuery(tableSql, null);
        c.moveToFirst();
        return c.getType(c.getColumnIndex(labalName));
    }

    /**
     * 关闭终端参数数据库文件
     */
    public void closeTermParamDb() {
        //关闭当前数据库
        if (database.isOpen()) {
            database.close();
        }
    }


    public OperatorParamOperation operatorParamOperation = new OperatorParamOperation();

    /**
     * 操作员类
     */
    public class OperatorParamOperation {

        public boolean update(String tableName, ContentValues cv, String OperatorNo) {
            String whereClause;
            String[] whereArgs;
            if (tableName == null || cv == null || OperatorNo == null) {
                Log.e(TAG, "请输入正确的参数");
                return false;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openOrCreateDatabase();
            }

            boolean ext;
            try {
                ext = isOperatorNoExist(tableName, OperatorNo);
            } catch (Exception e) {
                e.printStackTrace();
                return false;//数据库异常
            }
            //如果操作员号存在就更新数据，否则插入数据
            if (!ext) {
                if (!cv.containsKey("OperatorNo")) {
                    cv.put("OperatorNo", OperatorNo);
                }
                database.insert(tableName, null, cv);
            } else {
                whereClause = "OperatorNo = ?";
                whereArgs = new String[]{OperatorNo};
                //更新数据
                int count = database.update(tableName, cv, whereClause, whereArgs);
                Log.e(TAG, String.valueOf(count));
            }
            return true;
        }

        /**
         * 获取终端参数中的通讯参数，统一以String类型返回，涉及到bool类型的true以“1”,false以
         * “0”返回。无值返回 null
         *
         * @param tableName  数据库表
         * @param OperatorNo 操作员号
         * @param labels     参数的标签
         * @return 返回标签对应的value
         */
        public ContentValues getData(String tableName, String OperatorNo, String... labels) {

            ContentValues values = new ContentValues();
            String selection = "OperatorNo = ?";
            String[] selectionArgs = new String[]{OperatorNo};

            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openOrCreateDatabase();
            }
            Cursor cursor = database.query(tableName, null, selection, selectionArgs, null, null, null);

            if (cursor.getCount() != 1) {
                Log.e(TAG, "数据库异常;存在同一流水号的多条数据");
                return null;
            }
            cursor.moveToFirst();
            for (String label : labels) {
                String value = cursor.getString(cursor.getColumnIndex(label));
                values.put(label, value);
            }

            return values;
        }

        /**
         * 统计数据库记录条数
         */
        public long recordNums(String tableName) {

            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openOrCreateDatabase();
            }

            // 调用查找并返回数据源
            Cursor cursor = database.rawQuery("select count(*)from " + tableName, null);
            //游标移到第一条记录准备获取数据
            cursor.moveToFirst();
            // 获取数据中的LONG类型数据
            long count = cursor.getLong(0);
            return count;
        }

        /**
         * 获取终端参数中的通讯参数，统一以String类型返回，涉及到bool类型的true以“1”,false以
         * “0”返回。无值返回 null
         *
         * @param tableName 数据库表
         * @param labalName 参数的标签
         * @return 返回标签对应的value
         */
        public String getData(String tableName, String labalName, String OperatorNo) {

            String value;
            String selection = "OperatorNo = ?";
            String[] selectionArgs = new String[]{OperatorNo};

            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openOrCreateDatabase();
            }
            Cursor cursor = database.query(tableName, null, selection, selectionArgs, null, null, null);

            if (cursor.getCount() != 1) {
                Log.e(TAG, "OperatorNo = " + OperatorNo);
                Log.e(TAG, "Count = " + cursor.getCount());
//                Log.e(TAG,"数据库异常;存在同一流水号的多条数据");
                return null;
            }
            cursor.moveToFirst();
            value = cursor.getString(cursor.getColumnIndex(labalName));
            return value;
        }

        public boolean update(String tableName, String labalName, String value, String OperatorNo) {
            String whereClause;
            String[] whereArgs;
            if (tableName == null || labalName == null || labalName.equals("pid") || OperatorNo == null) {
                Log.e(TAG, "请输入正确的参数");
                return false;
            }
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openOrCreateDatabase();
            }

            boolean ext;
            ContentValues cv = new ContentValues();
            cv.put(labalName, value);
            try {
                ext = isOperatorNoExist(tableName, OperatorNo);
            } catch (Exception e) {
                e.printStackTrace();
                return false;//数据库异常
            }
            //如果操作员号存在就更新数据，否则插入数据
            if (!ext) {
                cv.put("OperatorNo", OperatorNo);
                database.insert(tableName, null, cv);
            } else {
                whereClause = "OperatorNo = ?";
                whereArgs = new String[]{OperatorNo};
                //更新数据
                int count = database.update(tableName, cv, whereClause, whereArgs);
                Log.e(TAG, String.valueOf(count));
            }
            return true;
        }

        /**
         * 查询操作员
         */
        public ContentValues queryOperator() {
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openOrCreateDatabase();
            }

            ContentValues cv = new ContentValues();
            //参数1：表名
            //参数2：要想显示的列
            //参数3：where子句
            //参数4：where子句对应的条件值
            //参数5：分组方式
            //参数6：having条件
            //参数7：排序方式
            Cursor cursor = database.query("OperatorParam", new String[]{"OperatorNo", "OperatorKey"}, "id=?", new String[]{"1"}, null, null, null);
            while (cursor.moveToNext()) {
                String OperatorNo = cursor.getString(cursor.getColumnIndex("OperatorNo"));
                String OperatorKey = cursor.getString(cursor.getColumnIndex("OperatorKey"));
                cv.put(OperatorNo, OperatorKey);
                System.out.println("query------->" + "操作员号：" + OperatorNo + " " + "操作员密码：" + OperatorKey);
            }
            return cv;
        }

        /*
        * 判断对操作员号的数据记录是否存在
        * */
        private boolean isOperatorNoExist(String tableName, String OperatorNo) throws Exception {
            String selection = "OperatorNo = ?";
            String[] selectionArgs = new String[]{OperatorNo};

            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openOrCreateDatabase();
            }
            Cursor cursor = database.query(tableName, null, selection, selectionArgs, null, null, null);
            int count = cursor.getCount();
            if (count > 1) {
                throw new Exception("数据库错误，一个操作员号存在多条记录");
            }
            if (count == 1) {
                return true;
            } else {
                return false;
            }
        }

        /*删除所有记录*/
        public int deleteAll(String tableName) {
            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openOrCreateDatabase();
            }
            return database.delete(tableName, null, null);
        }

        /*删除交易明细表的一条记录*/
        public boolean delete(String tableName, String OperatorNo) {
            if (tableName == null || OperatorNo == null) { return false;}

            //如果数据库没打开就打开它
            if (!database.isOpen()) {
                database = openOrCreateDatabase();
            }
            String selection = "OperatorNo = ?";
            String[] selectionArgs = new String[]{OperatorNo};
            boolean ext;
            try {
                ext = isOperatorNoExist(tableName, OperatorNo);
            } catch (Exception e) {
                ext = false;
                e.printStackTrace();
            }
            if (!ext) {
                Log.e(TAG, "删除数据记录失败，不存在这条记录");
                return false;
            }
            database.delete(tableName, selection, selectionArgs);
            return true;
        }
    }
}
