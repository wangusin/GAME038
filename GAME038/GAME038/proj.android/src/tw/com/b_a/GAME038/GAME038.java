/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package tw.com.b_a.GAME038;

import java.io.File;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.annotation.SuppressLint;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.AssetFileDescriptor;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;

public class GAME038 extends Cocos2dxActivity{

	public static GAME038 selfActivity = null;
    private static MediaPlayer mediaPlayer = null;
	
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
	}
	
    static {
         System.loadLibrary("game");
    }
    
    //playLongSound
    public static String getExternalStorageDirectory()
    {
    	try
        {
			PackageManager pm = selfActivity.getPackageManager();
			PackageInfo pi= pm.getPackageInfo(selfActivity.getPackageName(), 0);
			int version = pi.versionCode;
			String mainFileName = "main." + version + "." + selfActivity.getPackageName() + ".obb";
            
			File root = Environment.getExternalStorageDirectory();
			File expPath = new File(root.toString() + "/Android/obb/"
                                    + selfActivity.getPackageName());
			
		    String s="";
		    s = expPath.toString() + "/" + mainFileName;
		    return(s);
		} catch (NameNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    	return null;
    }
    
    public static void playLongSound(final String msg)
    {
    	if (mediaPlayer != null)
    	{
    		mediaPlayer.release();
    	}
    	
    	try {
    		AssetFileDescriptor fd;
    		fd = getContext().getAssets().openFd(msg);
            if (mediaPlayer != null){
            	mediaPlayer.stop();
            	mediaPlayer.release();
			}
            
            mediaPlayer = new MediaPlayer();
            
            mediaPlayer.setDataSource(fd.getFileDescriptor(),fd.getStartOffset(), fd.getLength());
            
			mediaPlayer.prepare();
			
			mediaPlayer.setLooping(false);
            
			mediaPlayer.start();
		} catch (final Exception e) {
			mediaPlayer = null;
			Log.e("PlayLongSound", "error: " + e.getMessage(), e);
		}
    }
    
    public static void stopLongSound()
    {
        if(mediaPlayer!=null)
        {
        	mediaPlayer.stop();
        	mediaPlayer.release();
        	
        	mediaPlayer = null;
        }
    }
    
    public static void pauseLongSound()
    {
        
        if(mediaPlayer!=null)
        {
        	mediaPlayer.pause();
        }
        
    }
    
    public static void resumeLongSound()
    {
        
        if(mediaPlayer!=null)
        {
        	mediaPlayer.start();
        }
    }
    
    @SuppressLint("NewApi")  public static void resumeApp()
    {
    	Log.v("smartyDonkey", "resumeApp");

    	int flag = selfActivity.getWindow().getDecorView().getSystemUiVisibility();
    	int uiOptions = flag | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION;
    	selfActivity.getWindow().getDecorView().setSystemUiVisibility(uiOptions);
    	selfActivity.getWindow().getDecorView().refreshDrawableState();
    	
    	selfActivity.getWindow().getCurrentFocus().setSystemUiVisibility(uiOptions);
    	selfActivity.getWindow().getCurrentFocus().refreshDrawableState();
    }
    //play long sound end
}
