package com.nexgo.transflow;

import android.content.Context;
import android.content.res.Resources;
import android.text.TextUtils;

import java.lang.reflect.Field;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;

public abstract class ResContainer {
    private Map<String, SocializeResource> mResources;
    private Context mContext;
    private static String mPackageName = "";

    public ResContainer(Context var1, Map<String, SocializeResource> var2) {
        this.mResources = var2;
        this.mContext = var1;
    }

    public static void setPackageName(String var0) {
        mPackageName = var0;
    }

    public static int getResourceId(Context var0, ResContainer.ResType var1, String var2) {
        Resources var3 = var0.getResources();
        if(TextUtils.isEmpty(mPackageName)) {
            mPackageName = var0.getPackageName();
        }

        int var4 = var3.getIdentifier(var2, var1.toString(), mPackageName);
        if(var4 <= 0) {
            throw new RuntimeException("获取资源ID失败:(packageName=" + mPackageName + " type=" + var1 + " name=" + var2);
        } else {
            return var4;
        }
    }

    public static String getString(Context var0, String var1) {
        int var2 = getResourceId(var0, ResContainer.ResType.STRING, var1);
        return var0.getString(var2);
    }

    public synchronized Map<String, SocializeResource> batch() {
        if(this.mResources == null) {
            return this.mResources;
        } else {
            Set var1 = this.mResources.keySet();

            ResContainer.SocializeResource var4;
            for(Iterator var2 = var1.iterator(); var2.hasNext(); var4.mIsCompleted = true) {
                String var3 = (String)var2.next();
                var4 = (ResContainer.SocializeResource)this.mResources.get(var3);
                var4.mId = getResourceId(this.mContext, var4.mType, var4.mName);
            }

            return this.mResources;
        }
    }

    public static int[] getStyleableArrts(Context var0, String var1) {
        return getResourceDeclareStyleableIntArray(var0, var1);
    }

    private static final int[] getResourceDeclareStyleableIntArray(Context var0, String var1) {
        try {
            Field[] var2 = Class.forName(var0.getPackageName() + ".R$styleable").getFields();
            Field[] var3 = var2;
            int var4 = var2.length;

            for(int var5 = 0; var5 < var4; ++var5) {
                Field var6 = var3[var5];
                if(var6.getName().equals(var1)) {
                    int[] var7 = (int[])((int[])var6.get((Object)null));
                    return var7;
                }
            }
        } catch (Throwable var8) {
            var8.printStackTrace();
        }

        return null;
    }

    public static class SocializeResource {
        public ResContainer.ResType mType;
        public String mName;
        public boolean mIsCompleted = false;
        public int mId;

        public SocializeResource(ResContainer.ResType var1, String var2) {
            this.mType = var1;
            this.mName = var2;
        }
    }

    public static enum ResType {
        LAYOUT {
            public String toString() {
                return "layout";
            }
        },
        ID {
            public String toString() {
                return "id";
            }
        },
        DRAWABLE {
            public String toString() {
                return "drawable";
            }
        },
        STYLE {
            public String toString() {
                return "style";
            }
        },
        STRING {
            public String toString() {
                return "string";
            }
        },
        COLOR {
            public String toString() {
                return "color";
            }
        },
        DIMEN {
            public String toString() {
                return "dimen";
            }
        },
        RAW {
            public String toString() {
                return "raw";
            }
        },
        ANIM {
            public String toString() {
                return "anim";
            }
        },
        STYLEABLE {
            public String toString() {
                return "styleable";
            }
        };

        private ResType() {
        }
    }
}