package com.mitac.touch;

import java.io.FileInputStream;
import java.io.FileNotFoundException;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Toast;

public class TouchDebugger extends Activity {
    
    private String m_strResult;
    private String m_strRootPath = "/sdcard/zinitix_fw.bin";
    private AlertDialog.Builder m_dlgWarning;
    private AlertDialog.Builder m_dlgResult;
    
    static { 
        System.loadLibrary("touch_debugger");
    }
    
    public native static int GetRegister(int addr);
    public native static int SetRegister(int addr, int value);
    public native static int SetRegisterSeq(int addr, int value);
    public native static int SendSaveStatus();
    public native static int GetXNodeNum();
    public native static int GetYNodeNum();
    public native static int DoHwCalibration();
    public native static int NoTouchEvent();
    public native static int SetRawDataMode(int val);
    public native static int GetRawData(byte[] rawData, int sz);
    public native static int GetChipRevision();
    public native static int GetFWversion();
    public native static int GetDataVersion();
    public native static int SetDebugMsg(int value);
    public native static int GetDebugMsgState();
    public native static int GetXResolution();
    public native static int GetYResolution();
    public native static String AdbGetXNodeNum();
    public native static int VerifyUpgradeData(byte[] rawData);
    public native static String StartUpgrade();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_touch_debugger);
    }

    public void onClick(View v) {
        try {
            int sz;
            int nResult;
            FileInputStream fis;
            int version;
            String str = "";
            fis = new FileInputStream(m_strRootPath);
            byte[] data = new byte[fis.available()];
            sz = fis.available();
            while (fis.read(data) != -1) {;}

            nResult = VerifyUpgradeData(data);

            m_strResult = "unkown error";
            if(nResult == -1)
                m_strResult = "dev file error ";
            else if(nResult == -2) {
                str = String.format(" (size = %d) ", sz);
                m_strResult = "invalid firmware file size" + str;
            } else if(nResult == -3)
                m_strResult = "invalid firmware file";
            else {
                version = nResult;
                str = String.format("firmware size = %d kb, firmware fersion : 0x%04X", sz/1024, version);
                nResult = 0;
            }
/*            if(nResult < 0) {
                Toast.makeText(getApplicationContext(),
                        m_strResult, Toast.LENGTH_SHORT).show();
                return;
            }
*/
            m_dlgWarning = new AlertDialog.Builder(TouchDebugger.this);
            m_dlgWarning.setTitle("Warning");
            m_dlgWarning.setMessage("Don't touch anything while upgrading");
            m_dlgWarning.setPositiveButton("Ok", new DialogInterface.OnClickListener() {                               
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    int result;

                    m_dlgResult = new AlertDialog.Builder(TouchDebugger.this);
                    m_strResult = "Upgrade Result : " + StartUpgrade();
                    m_dlgResult.setTitle("UPGRADE RESULT");                                                            
                    m_dlgResult.setMessage(m_strResult);
                    m_dlgResult.setPositiveButton("Exit", new DialogInterface.OnClickListener() {                      
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            dialog.cancel();
                        }
                    });
                    //mMenuText[0] = getICInfomation();
                    //listView = (ListView)findViewById(R.id.upgradelistview);
                    //listView.setAdapter(new ImageList(UpgradeManager.this, mMenuText));

                    m_dlgResult.show();
                }
            });
            m_dlgWarning.setNegativeButton("No", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    dialog.cancel();
                }
            });

            m_dlgWarning.show();
            //AlertDialog m_dlgWarngAlert = m_dlgWarning.create();
            //m_dlgWarngAlert.show();

      } catch (FileNotFoundException e) {
        Toast.makeText(getApplicationContext(),
             m_strRootPath + " is not found",
             Toast.LENGTH_SHORT).show();
      } catch (Exception e) {
        Toast.makeText(getApplicationContext(),
             "Exception: " + e.toString(),
             Toast.LENGTH_SHORT).show();
      }
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.touch_debugger, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
