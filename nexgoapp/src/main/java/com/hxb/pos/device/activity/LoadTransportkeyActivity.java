package com.hxb.pos.device.activity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.hxb.pos.device.R;
import com.hxb.pos.device.utils.AppGlobal;
import com.nexgo.common.ByteUtils;
import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.DeviceApusic;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Created by zhanbiqiang
 * Email: zhanbiqiang@xinguodu.com
 * Date: 2016/4/22 14:34
 * Describe:
 */
public class LoadTransportkeyActivity extends Activity {
    private EditText edit_PMK,edit_KLKIndex,edit_PMKeyId;
    private Button btn_ok;
    private int KLKIndex = 1;//传输密钥索引
    private TradeTlv tradeTlv;
    private String strKLK = "D84EF09AB40A638E7BE1DBB1F3006528";
    private String strPMKeyId = "002200";
    private String strKLKIndex = "1";

    private Logger log = LoggerFactory.getLogger("LoadTransportkeyActivity");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_load_primary_key);
        edit_PMK = (EditText)findViewById(R.id.input_PMK);
        edit_KLKIndex = (EditText) findViewById(R.id.input_PMK_index);
        edit_PMKeyId = (EditText) findViewById(R.id.PMKeyId);
        tradeTlv = TradeTlv.getInstance();
        btn_ok = (Button) findViewById(R.id.btn_ok);
        btn_ok.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                DeviceApusic deviceApusic = new DeviceApusic();
                if(!"".equals(edit_PMK.getText().toString())){
                    strKLK = edit_PMK.getText().toString();
                }

                if(!"".equals(edit_PMKeyId.getText().toString())){
                    strPMKeyId = edit_PMKeyId.getText().toString();
                }

                if(!"".equals(edit_KLKIndex.getText().toString())){
                    strKLKIndex = edit_KLKIndex.getText().toString();
                }

                KLKIndex = Integer.parseInt(strKLKIndex);
                tradeTlv.setTagValue(TradeTlv.Klkindex, strKLKIndex.getBytes());
                tradeTlv.setTagValue(tradeTlv.PMKId, strPMKeyId.getBytes());

                byte[] tmp = ByteUtils.hexString2ByteArray(strKLK);
//                log.debug("getBCDString:{}", ByteUtils.(ByteUtils.getBCDString(strKLK)));

                log.debug("tmp:{}",ByteUtils.byteArray2HexString(tmp));

                deviceApusic.LoadKLK(tmp, KLKIndex);
                Log.d("LoadTransportkey","手工输入的传输密钥明文 = "+ strKLK);
                Intent intent = new Intent();
                intent.putExtra("tradeCode", "0");//成功
                try {
                    AppGlobal.getInstance().getTransactionResult().onResult(intent);
                    finish();
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            }
        });
    }
}
