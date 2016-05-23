package com.hxb.pos.device.fragment;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import com.hxb.pos.device.R;

/**
 * 作者:liuting on 2016/3/17 18:57
 * 邮箱:liuting@xinguodu.com
 * 项目名：ump
 * 包名：com.xgd.umsapp.fragment
 * TODO:
 */
public class ResponseSuccessFragment extends BaseFragment {
    @Override
    protected View initView() {
        View view=LayoutInflater.from(mContext).inflate(R.layout.fragment_responsesuccess,null);
        TextView textView= (TextView) view.findViewById(R.id.tv_responsemsg_success_fragment);
        Bundle bundle=getArguments();
        String tip= (String) bundle.get("tip");
        textView.setText(tip);
        return view;

    }
}
