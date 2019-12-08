package com.jiesiren.catcam;

import android.app.NativeActivity;
import android.os.Bundle;
import android.util.Log;
import org.jetbrains.annotations.Nullable;

public final class MainActivity extends NativeActivity {

    private final String TAG = "CatCam.MainActivity";

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(TAG, "onCreate()");
        setContentView(R.layout.activity_main);
    }

    static {
        System.loadLibrary("CatCam");
    }
}
