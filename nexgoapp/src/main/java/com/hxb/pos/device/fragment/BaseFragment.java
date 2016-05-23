package com.hxb.pos.device.fragment;

import android.content.Context;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

/**
 * 作者:liuting on 2016/3/10 14:55
 * 邮箱:liuting@xinguodu.com
 * 项目名：
 * 包名：
 * TODO:fragment的基类
 */
public abstract class BaseFragment extends Fragment {
    protected Context mContext;
    @Override
    public void onAttach(Context context){
        super.onAttach(context);
        mContext=context;
    }
    @Override
    public void onActivityCreated(@Nullable Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        initData();
    }
    protected  void initData(){

    }
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return initView();
    }

    protected abstract View initView();
}
