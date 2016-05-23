/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: D:\\hxb\\app\\src\\main\\aidl\\com\\hxb\\pos\\device\\service\\TransactionControllerAidl.aidl
 */
package com.hxb.pos.device.service;
public interface TransactionControllerAidl extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements com.hxb.pos.device.service.TransactionControllerAidl
{
private static final java.lang.String DESCRIPTOR = "com.hxb.pos.device.service.TransactionControllerAidl";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an com.hxb.pos.device.service.TransactionControllerAidl interface,
 * generating a proxy if needed.
 */
public static com.hxb.pos.device.service.TransactionControllerAidl asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof com.hxb.pos.device.service.TransactionControllerAidl))) {
return ((com.hxb.pos.device.service.TransactionControllerAidl)iin);
}
return new com.hxb.pos.device.service.TransactionControllerAidl.Stub.Proxy(obj);
}
@Override public android.os.IBinder asBinder()
{
return this;
}
@Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
{
switch (code)
{
case INTERFACE_TRANSACTION:
{
reply.writeString(DESCRIPTOR);
return true;
}
case TRANSACTION_common:
{
data.enforceInterface(DESCRIPTOR);
java.lang.String _arg0;
_arg0 = data.readString();
java.util.Map _arg1;
java.lang.ClassLoader cl = (java.lang.ClassLoader)this.getClass().getClassLoader();
_arg1 = data.readHashMap(cl);
com.hxb.pos.device.service.TransactionResult _arg2;
_arg2 = com.hxb.pos.device.service.TransactionResult.Stub.asInterface(data.readStrongBinder());
this.common(_arg0, _arg1, _arg2);
reply.writeNoException();
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements com.hxb.pos.device.service.TransactionControllerAidl
{
private android.os.IBinder mRemote;
Proxy(android.os.IBinder remote)
{
mRemote = remote;
}
@Override public android.os.IBinder asBinder()
{
return mRemote;
}
public java.lang.String getInterfaceDescriptor()
{
return DESCRIPTOR;
}
@Override public void common(java.lang.String transactionName, java.util.Map para, com.hxb.pos.device.service.TransactionResult transactionResult) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeString(transactionName);
_data.writeMap(para);
_data.writeStrongBinder((((transactionResult!=null))?(transactionResult.asBinder()):(null)));
mRemote.transact(Stub.TRANSACTION_common, _data, _reply, 0);
_reply.readException();
}
finally {
_reply.recycle();
_data.recycle();
}
}
}
static final int TRANSACTION_common = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
}
public void common(java.lang.String transactionName, java.util.Map para, com.hxb.pos.device.service.TransactionResult transactionResult) throws android.os.RemoteException;
}
