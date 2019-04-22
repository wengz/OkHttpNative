package com.wengzc.okhttpnative;

import android.Manifest;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.io.IOException;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        checkPermission();

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());
        tv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                testOkHttp();
            }
        });
    }


    private void checkPermission (){
        if (PermissionUtil.needRequestPermissions(this, Manifest.permission.INTERNET)){
            PermissionUtil.requestPermissions(this, 7788, Manifest.permission.INTERNET);
        }
    }

    private void testOkHttp (){
        new Thread(){

            @Override
            public void run() {

                try{
                    OkHttpClient client = new OkHttpClient();
                    Request request = new Request.Builder()
                            .url("http://www.baidu.com")
                            .build();

                    Response response = client.newCall(request).execute();
                    String content = response.body().string();
                    Log.d("ooxx", "run: "+content);
                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        }.start();

    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
}
