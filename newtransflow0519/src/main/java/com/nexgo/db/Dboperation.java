package com.nexgo.db;

import android.content.ContentValues;
import android.database.sqlite.SQLiteDatabase;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * Created by HGD on 2016/3/12.
 * 数据库操作接口
 */
public interface Dboperation {
    SQLiteDatabase openOrCreateDatabase();

    void beginTransaction();

    void setTransactionSuccessful();

    void endTransaction();

    boolean update(ContentValues cv, String VoucherNO);

    boolean update(String labalName, String value, String VoucherNO);

    ContentValues getData(String[] labels, String VoucherNO);

    String getData(String labalName, String VoucherNO);

    ContentValues getOneRecord(String VoucherNO);

    int deleteAll();

    boolean delete(String VoucherNO);

    int getrecordNums();

    int isVoucherNOExist(String VoucherNO);

    boolean updateReversalTbale(String labalName, String value);

    String getReversalData(String labalName);

    HashMap settleResult(String tradeType, String cardType);

    HashMap settleResult(String tradeId);

    HashMap settleStatistics(String tradeId,String cardFlag);
    HashMap settleStatistics(String tradeId,int uploadstate);

    String getScriptData(String labalName);

    boolean updateScriptTbale(String labalName, String value);

    ArrayList<ContentValues> getSucessConsumeInfo(int index);

    ArrayList<ContentValues> getAllInfo(int index);

    ContentValues getOneRecordByIndex(int index);

    String getOtherInfoTableData(String labalName);

    boolean updateOtherInfoTable(String labalName, String value);
}
