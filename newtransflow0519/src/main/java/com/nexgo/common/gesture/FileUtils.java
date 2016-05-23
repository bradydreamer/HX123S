package com.nexgo.common.gesture;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.os.Environment;
import android.os.StatFs;
import android.util.Log;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class FileUtils {
    private static int bufferd = 1024;

    private FileUtils() {
    }

    // sd���Ƿ����
    public static boolean isSdcardAvailable() {
        String status = Environment.getExternalStorageState();
        if (status.equals(Environment.MEDIA_MOUNTED)) {
            return true;
        }
        return false;
    }

    /**
     * ��ȡsd����С *
     */
    public static long getSDAllSizeKB() {
        // get path of sdcard
        File path = Environment.getExternalStorageDirectory();
        StatFs sf = new StatFs(path.getPath());
        // get single block size(Byte)
        long blockSize = sf.getBlockSize();
        // ��ȡ������ݿ���
        long allBlocks = sf.getBlockCount();
        // ����SD����С
        return (allBlocks * blockSize) / 1024; // KB
    }

    /**
     * �ļ��Ƿ����
     */
    public static boolean isFileExist(String fileName) {
        File file = new File(Environment.getExternalStorageDirectory()
                + File.separator + fileName);
        return file.exists();
    }

    public static boolean isFileExist(String director, String fileName) {
        File file = new File(Environment.getExternalStorageDirectory()
                + File.separator + fileName);
        return file.exists();
    }

    /**
     * create multiple director
     *
     * @param path lujing
     * @return
     */
    public static boolean createDir(String director) {
        if (isFileExist(director)) {
            return true;
        } else {
            File file = new File(Environment.getExternalStorageDirectory()
                    + File.separator + director);
            if (!file.exists()) {
                file.mkdirs();
                return false;
            }
            return true;
        }
    }

    /**
     * ɾ���ļ�
     */
    public void deleteFile(File file) {
        if (file.exists()) { // �ж��ļ��Ƿ����
            if (file.isFile()) { // �ж��Ƿ����ļ�
                file.delete(); // delete()���� ��Ӧ��֪�� ��ɾ�����˼;
            } else if (file.isDirectory()) { // �����������һ��Ŀ¼
                File files[] = file.listFiles(); // ����Ŀ¼�����е��ļ� files[];
                for (int i = 0; i < files.length; i++) { // ����Ŀ¼�����е��ļ�
                    this.deleteFile(files[i]); // ��ÿ���ļ� ������������е��
                }
            }
            file.delete();
        } else {

        }
    }

    /**
     * bitmap����ΪbmpͼƬ
     */
//    public static File writeToSDCardFromBitmap(Bitmap bitmap, String fileName) {
//        return writeToSDCardFromBitmap(bitmap, "XGDhardWrite", fileName);
//    }

    public static File writeToSDCardFromBitmap(Bitmap bitmap, String directory,
                                               String fileName) {
        File file = null;
        String filename;  //带完整路径的文件名
        OutputStream os = null;

        try {
            createDir(directory);
            filename = Environment.getExternalStorageDirectory() +
                    File.separator + directory + File.separator + fileName;

//            file = new File(Environment.getExternalStorageDirectory(),
//                    directory + File.separator + fileName);
            file = new File(filename);
            if (!file.exists())
                file.createNewFile();
            os = new FileOutputStream(file);
            int w = bitmap.getWidth();
            int h = bitmap.getHeight();
            int[] pixels = new int[w * h];
            bitmap.getPixels(pixels, 0, w, 0, 0, w, h);

//            byte[] rgb = addBMP_RGB_888(pixels, w, h);
//            byte[] header = addBMPImageHeader(rgb.length);
//            byte[] infos = addBMPImageInfosHeader(w, h);
//
//            byte[] buffer = new byte[54 + rgb.length];
//            System.arraycopy(header, 0, buffer, 0, header.length);
//            System.arraycopy(infos, 0, buffer, 14, infos.length);
//            System.arraycopy(rgb, 0, buffer, 54, rgb.length);
//            int gray;
//            for (int j = 0; j < h; j++) {// 扫描列
//                for (int i = 0; i < w; i++) // 扫描行
//                { // 由红，绿，蓝值得到灰度值
//                    gray = (int) (((pixels[w * j + i] >> 16) & 0xff) * 0.8);
//                    gray += (int) (((pixels[w * j + i] >> 8) & 0xff) * 0.1);
//                    gray += (int) (((pixels[w * j + i]) & 0xff) * 0.1);
//                    pixels[w * j + i] = (255 << 24) | (gray << 16) | (gray << 8)
//                            | gray;
//                }
//            }
            Bitmap bm = Bitmap.createBitmap(w, h, Bitmap.Config.RGB_565);
            bm.setPixels(pixels, 0, w, 0, 0, w, h);
//            int numBytesByRow = bm.getRowBytes() * bm.getHeight();
//            ByteBuffer byteBuffer = ByteBuffer.allocate(numBytesByRow);
//            bm.copyPixelsToBuffer(byteBuffer);
//            byte[] bytes = byteBuffer.array();
//
//            FileOutputStream fileOuputStream = new FileOutputStream("/sdcard/XGDhardWrite/2.bmp");
//            fileOuputStream.write(bytes);
//            fileOuputStream.close();
            saveBmp(bm, filename);
//            saveBmpTest(bm);

//            if (in != null) {
//                in.close();
//            }
//            if (buf != null) {
//                buf.close();
//            }

//            bm.compress(Bitmap.CompressFormat.JPEG,100,os);
//            os.write(bm);
//            os.flush();
        } catch (Exception e) {
            Log.e("FileUtil", "" + e.getMessage());
            e.printStackTrace();
        } finally {
            try {
                os.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return file;

    }

    /**
     * bitmap����ΪbmpͼƬ��Ӧ�ķ���
     */

    private static byte[] addBMP_RGB_888(int[] b, int w, int h) {
        int len = b.length;

        byte[] buffer = new byte[w * h * 4];
        int offset = 0;
        for (int i = len - 1; i >= w; i -= w) {
            int end = i, start = i - w + 1;
            for (int j = start; j <= end; j++) {
                buffer[offset] = (byte) (b[j] >> 0);
                buffer[offset + 1] = (byte) (b[j] >> 8);
                buffer[offset + 2] = (byte) (b[j] >> 16);
                buffer[offset + 3] = (byte) (b[j] >> 24);
                offset += 4;
            }
        }
        return buffer;
    }

    // BMP�ļ���Ϣͷ
    private static byte[] addBMPImageInfosHeader(int w, int h) {
        byte[] buffer = new byte[40];
        // ����ǹ̶��� BMP ��ϢͷҪ40���ֽ�
        buffer[0] = 0x28;
        buffer[1] = 0x00;
        buffer[2] = 0x00;
        buffer[3] = 0x00;
        // ��� ��λ�������ǰ��λ�� ��λ������ź��λ��
        buffer[4] = (byte) (w >> 0);
        buffer[5] = (byte) (w >> 8);
        buffer[6] = (byte) (w >> 16);
        buffer[7] = (byte) (w >> 24);
        // ���� ͬ��
        buffer[8] = (byte) (h >> 0);
        buffer[9] = (byte) (h >> 8);
        buffer[10] = (byte) (h >> 16);
        buffer[11] = (byte) (h >> 24);
        // ���Ǳ�����Ϊ1
        buffer[12] = 0x01;
        buffer[13] = 0x00;
        // ������ ���� 32λ����һ������ �����ͬ�ķ�ʽ(ARGB 32λ RGB24λ��ͬ��!!!!)
        buffer[14] = 0x20;
        buffer[15] = 0x00;
        // 0-��ѹ�� 1-8bitλͼ
        // 2-4bitλͼ 3-16/32λͼ
        // 4 jpeg 5 png
        buffer[16] = 0x00;
        buffer[17] = 0x00;
        buffer[18] = 0x00;
        buffer[19] = 0x00;
        // ˵��ͼ���С
        buffer[20] = 0x00;
        buffer[21] = 0x00;
        buffer[22] = 0x00;
        buffer[23] = 0x00;
        // ˮƽ�ֱ���
        buffer[24] = 0x00;
        buffer[25] = 0x00;
        buffer[26] = 0x00;
        buffer[27] = 0x00;
        // ��ֱ�ֱ���
        buffer[28] = 0x00;
        buffer[29] = 0x00;
        buffer[30] = 0x00;
        buffer[31] = 0x00;
        // 0 ʹ�����еĵ�ɫ����
        buffer[32] = 0x00;
        buffer[33] = 0x00;
        buffer[34] = 0x00;
        buffer[35] = 0x00;
        // ������ɫ����
        buffer[36] = 0x00;
        buffer[37] = 0x00;
        buffer[38] = 0x00;
        buffer[39] = 0x00;
        return buffer;
    }

    // BMP�ļ�ͷ
    private static byte[] addBMPImageHeader(int size) {
        byte[] buffer = new byte[14];
        // magic number 'BM'
        buffer[0] = 0x42;
        buffer[1] = 0x4D;
        // ��¼��С
        buffer[2] = (byte) (size >> 0);
        buffer[3] = (byte) (size >> 8);
        buffer[4] = (byte) (size >> 16);
        buffer[5] = (byte) (size >> 24);
        buffer[6] = 0x00;
        buffer[7] = 0x00;
        buffer[8] = 0x00;
        buffer[9] = 0x00;
        buffer[10] = 0x36;
        buffer[11] = 0x00;
        buffer[12] = 0x00;
        buffer[13] = 0x00;
        return buffer;
    }


    /**
     * 将Bitmap存为 .bmp格式图片
     *
     * @param bitmap
     */
    private static void saveBmp(Bitmap bitmap, String filename) {
        if (bitmap == null)
            return;
        // 位图大小
        int nBmpWidth = bitmap.getWidth();
        int nBmpHeight = bitmap.getHeight();
        // 图像数据大小
        int biBitCount = 1;
        int pixSize = nBmpWidth * nBmpHeight;
        long biSizeImage = pixSize * biBitCount / 8;
        try {
            // 存储文件名
//            String filename = "/sdcard/XGDhardWrite/1.bmp";
            File file = new File(filename);
            if (!file.exists()) {
                file.createNewFile();
            }
            FileOutputStream fileos = new FileOutputStream(filename);
            // bmp文件头
            int bfType = 0x4d42;
            long bfOffBits = 14 + 40 + 8;
            long bfSize = biSizeImage + bfOffBits;
            int bfReserved1 = 0;
            int bfReserved2 = 0;
            // 保存bmp文件头
            writeWord(fileos, bfType);
            writeDword(fileos, bfSize);
            writeWord(fileos, bfReserved1);
            writeWord(fileos, bfReserved2);
            writeDword(fileos, bfOffBits);
            // bmp信息头
            long biSize = 40L;
            long biWidth = nBmpWidth;
            long biHeight = nBmpHeight;
            int biPlanes = 1;

            long biCompression = 0L;

            long biXpelsPerMeter = 0L;
            long biYPelsPerMeter = 0L;
            long biClrUsed = 0L;
            long biClrImportant = 2L;
            // 保存bmp信息头
            writeDword(fileos, biSize);
            writeLong(fileos, biWidth);
            writeLong(fileos, biHeight);
            writeWord(fileos, biPlanes);
            writeWord(fileos, biBitCount);
            writeDword(fileos, biCompression);
            writeDword(fileos, biSizeImage);
            writeLong(fileos, biXpelsPerMeter);
            writeLong(fileos, biYPelsPerMeter);
            writeLong(fileos, biClrUsed);
            writeLong(fileos, biClrImportant);
            writeLong(fileos,0xff000000L);
            writeLong(fileos,0xffffffffL);

            // 像素扫描
            int wWidth = (nBmpWidth * 3 + nBmpWidth % 4);
            int bufferSize = wWidth * nBmpHeight;
            int bmpData[] = new int[bufferSize];

            int dataSize = bufferSize / 24;
            byte data[] = new byte[dataSize];
            for (int i = 0; i< dataSize; i++) {
                data[i] = 0;
            }

            for (int nCol = 0, nRealCol = nBmpHeight - 1; nCol < nBmpHeight; ++nCol, --nRealCol) {
                for (int wRow = 0, wByteIdex = 0; wRow < nBmpWidth; wRow++, wByteIdex += 3) {
                    int clr = bitmap.getPixel(wRow, nCol);
                    bmpData[nRealCol * wWidth + wByteIdex] =  Color.blue(clr);
                    bmpData[nRealCol * wWidth + wByteIdex + 1] =  Color.green(clr);
                    bmpData[nRealCol * wWidth + wByteIdex + 2] =  Color.red(clr);

                    int tmp = Color.green(clr) > 128 ? 1 : 0;
                    if (tmp == 1) {
                        data[(nRealCol * wWidth + wByteIdex) / 24] |= 1 << (7 - (((nRealCol * wWidth + wByteIdex) / 3) % 8));
                    }
                }
            }
/*
            for (int i = 0; i < dataSize; i++) {
                int tmpData = 0;
                for (int j = 0; j < 8; j++) {
                    int tmp = ((bmpData[i * 8 + j * 3] > 128) ? 1 : 0);
                    if (tmp == 1) {
                        tmpData |= 0x01 << (7-j);
                    }
                }
                data[i] = (byte) tmpData;
            }
*/

            fileos.write(data);
            fileos.flush();
            fileos.close();

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    private static void saveBmpTest(Bitmap bitmap) {
        if (bitmap == null)
            return;
        // 位图大小
        int nBmpWidth = bitmap.getWidth();
        int nBmpHeight = bitmap.getHeight();
        // 图像数据大小
        int bufferSize = nBmpHeight * (nBmpWidth * 3 + nBmpWidth % 4);
        try {
            // 存储文件名
            String filename = "/sdcard/XGDhardWrite/test.bmp";
            File file = new File(filename);
            if (!file.exists()) {
                file.createNewFile();
            }
            FileOutputStream fileos = new FileOutputStream(filename);
            // bmp文件头
            int bfType = 0x4d42;
            long bfSize = 14 + 40 + bufferSize;
            int bfReserved1 = 0;
            int bfReserved2 = 0;
            long bfOffBits = 14 + 40;
            // 保存bmp文件头
            writeWord(fileos, bfType);
            writeDword(fileos, bfSize);
            writeWord(fileos, bfReserved1);
            writeWord(fileos, bfReserved2);
            writeDword(fileos, bfOffBits);
            // bmp信息头
            long biSize = 40L;
            long biWidth = nBmpWidth;
            long biHeight = nBmpHeight;
            int biPlanes = 1;
            int biBitCount = 24;
            long biCompression = 0L;
            long biSizeImage = 0L;
            long biXpelsPerMeter = 0L;
            long biYPelsPerMeter = 0L;
            long biClrUsed = 0L;
            long biClrImportant = 0L;
            // 保存bmp信息头
            writeDword(fileos, biSize);
            writeLong(fileos, biWidth);
            writeLong(fileos, biHeight);
            writeWord(fileos, biPlanes);
            writeWord(fileos, biBitCount);
            writeDword(fileos, biCompression);
            writeDword(fileos, biSizeImage);
            writeLong(fileos, biXpelsPerMeter);
            writeLong(fileos, biYPelsPerMeter);
            writeDword(fileos, biClrUsed);
            writeDword(fileos, biClrImportant);
            // 像素扫描
            byte bmpData[] = new byte[bufferSize];
            int wWidth = (nBmpWidth * 3 + nBmpWidth % 4);
            for (int nCol = 0, nRealCol = nBmpHeight - 1; nCol < nBmpHeight; ++nCol, --nRealCol){
                for (int wRow = 0, wByteIdex = 0; wRow < nBmpWidth; wRow++, wByteIdex += 3) {
                    int clr = bitmap.getPixel(wRow, nCol);
                    bmpData[nRealCol * wWidth + wByteIdex] = (byte) Color.blue(clr);
                    bmpData[nRealCol * wWidth + wByteIdex + 1] = (byte) Color.green(clr);
                    bmpData[nRealCol * wWidth + wByteIdex + 2] = (byte) Color.red(clr);
                }
            }
            fileos.write(bmpData);
            fileos.flush();
            fileos.close();

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static int gbs(int x, int y) {
        for (int i = 0; i < x * y; i++) {
            if (i % x == 0 && y == 0) {
                return i;
            }
        }
        return x * y;
    }

    protected static void writeWord(FileOutputStream stream, int value) throws IOException {
        byte[] b = new byte[2];
        b[0] = (byte) (value & 0xff);
        b[1] = (byte) (value >> 8 & 0xff);
        stream.write(b);
    }

    protected static void writeDword(FileOutputStream stream, long value) throws IOException {
        byte[] b = new byte[4];
        b[0] = (byte) (value & 0xff);
        b[1] = (byte) (value >> 8 & 0xff);
        b[2] = (byte) (value >> 16 & 0xff);
        b[3] = (byte) (value >> 24 & 0xff);
        stream.write(b);
    }

    protected static void writeLong(FileOutputStream stream, long value) throws IOException {
        byte[] b = new byte[4];
        b[0] = (byte) (value & 0xff);
        b[1] = (byte) (value >> 8 & 0xff);
        b[2] = (byte) (value >> 16 & 0xff);
        b[3] = (byte) (value >> 24 & 0xff);
        stream.write(b);
    }

}
