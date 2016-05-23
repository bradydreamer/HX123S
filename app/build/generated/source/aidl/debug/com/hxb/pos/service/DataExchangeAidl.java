/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: D:\\hxb\\app\\src\\main\\aidl\\com\\hxb\\pos\\service\\DataExchangeAidl.aidl
 */
package com.hxb.pos.service;
/**
 * 类说明：
 */
public interface DataExchangeAidl extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements com.hxb.pos.service.DataExchangeAidl
{
private static final java.lang.String DESCRIPTOR = "com.hxb.pos.service.DataExchangeAidl";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an com.hxb.pos.service.DataExchangeAidl interface,
 * generating a proxy if needed.
 */
public static com.hxb.pos.service.DataExchangeAidl asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof com.hxb.pos.service.DataExchangeAidl))) {
return ((com.hxb.pos.service.DataExchangeAidl)iin);
}
return new com.hxb.pos.service.DataExchangeAidl.Stub.Proxy(obj);
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
case TRANSACTION_exchangeData:
{
data.enforceInterface(DESCRIPTOR);
byte[] _arg0;
_arg0 = data.createByteArray();
byte[] _result = this.exchangeData(_arg0);
reply.writeNoException();
reply.writeByteArray(_result);
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements com.hxb.pos.service.DataExchangeAidl
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
@Override public byte[] exchangeData(byte[] data) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
byte[] _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeByteArray(data);
mRemote.transact(Stub.TRANSACTION_exchangeData, _data, _reply, 0);
_reply.readException();
_result = _reply.createByteArray();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
}
static final int TRANSACTION_exchangeData = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
}
public byte[] exchangeData(byte[] data) throws android.os.RemoteException;
}
