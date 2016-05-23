package com.hxb.pos.device.fragment;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import com.hxb.pos.device.R;
import com.nexgo.common.TradeTlv;


/**
 * 作者:liuting on 2016/3/17 18:57
 * 邮箱:liuting@xinguodu.com
 * 项目名：ump
 * 包名：com.xgd.umsapp.fragment
 * TODO:
 */
public class ResponseFailFragment extends BaseFragment {
    @Override
    protected View initView() {
        TradeTlv tradeTlv = TradeTlv.getInstance();
        byte[] tagValue = tradeTlv.getTagValue(TradeTlv.Repon);
        View view = LayoutInflater.from(mContext).inflate(R.layout.fragment_responsefail, null);
        TextView tv_code = (TextView) view.findViewById(R.id.tv_errorcode_response_error);
        TextView tv_response = (TextView) view.findViewById(R.id.tv_responsemsg_error);
        Bundle bunble = getArguments();
        String tip = (String) bunble.get("tip");
        tv_response.setText(tip);
        if (tagValue != null)
            tv_code.setText(new String(tagValue));
        return view;
    }
}
