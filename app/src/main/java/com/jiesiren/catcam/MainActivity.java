package com.jiesiren.catcam;

import android.Manifest;
import android.app.NativeActivity;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.ImageButton;
import android.widget.PopupWindow;
import android.widget.RelativeLayout;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;

import org.jetbrains.annotations.Nullable;

public final class MainActivity extends NativeActivity {

    static {
        System.loadLibrary("CatCam");
    }

    private final String TAG = "[JAVA] MainActivity";

    private final int PERMISSION_REQUEST_CODE = 1;

    private PopupWindow popupWindow;
    private ImageButton shutterButton;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.i(TAG, "onCreate()");

        hideSystemUi();
        View decorView = getWindow().getDecorView();
        decorView.setOnSystemUiVisibilityChangeListener
                (new View.OnSystemUiVisibilityChangeListener() {
                    @Override
                    public void onSystemUiVisibilityChange(int visibility) {
                        hideSystemUi();
                    }
                });
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemUi();
        }
    }

    @SuppressWarnings("unused") // Called through JNI
    public void requestPermissions() {
        final String[] requiredPermissions = new String[]{
                Manifest.permission.CAMERA,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
        };
        for (String permission : requiredPermissions) {
            final int checkResult = ActivityCompat.checkSelfPermission(this, permission);
            if (checkResult != PackageManager.PERMISSION_GRANTED) {
                Log.i(TAG, "Permission not granted for " + permission
                        + ", making a request.");
                ActivityCompat.requestPermissions(
                        this,
                        requiredPermissions,
                        PERMISSION_REQUEST_CODE);
                return;
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        if (requestCode == PERMISSION_REQUEST_CODE) {
            if (grantResults.length >= 2
                    && grantResults[0] == PackageManager.PERMISSION_GRANTED
                    && grantResults[1] == PackageManager.PERMISSION_GRANTED) {
                Log.i(TAG, "Permissions granted!");
            } else {
                // TODO: toast saying you need permissions!
                Log.i(TAG, "We need permissions to work!");
            }
        }
    }

    @SuppressWarnings("unused") // Called through JNI
    public void showAppUi() {
        Log.i(TAG, "Current thread: " + Thread.currentThread().getName());

        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Log.i(TAG, "In Runnable: Current thread: " + Thread.currentThread().getName());

                if (popupWindow != null) {
                    popupWindow.dismiss();
                }
                LayoutInflater layoutInflater = (LayoutInflater) getBaseContext()
                        .getSystemService(LAYOUT_INFLATER_SERVICE);
                View popupView = layoutInflater.inflate(R.layout.activity_main, null);
                popupWindow = new PopupWindow(
                        popupView,
                        WindowManager.LayoutParams.MATCH_PARENT,
                        WindowManager.LayoutParams.WRAP_CONTENT);

                RelativeLayout mainLayout = new RelativeLayout(MainActivity.this);
                ViewGroup.MarginLayoutParams params = new ViewGroup.MarginLayoutParams(
                        -1, -1);
                params.setMargins(0, 0, 0, 0);
                MainActivity.this.setContentView(mainLayout, params);

                // Show our UI over NativeActivity window
                popupWindow.showAtLocation(mainLayout, Gravity.BOTTOM | Gravity.START, 0, 0);
                popupWindow.update();

                shutterButton = popupView.findViewById(R.id.shutter_button);
                shutterButton.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        Log.i(TAG, "Shutter clicked!");
                    }
                });
                shutterButton.setEnabled(true);
            }
        });
    }

    private void hideSystemUi() {
        Log.i(TAG, "hideSystemUi");
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                        // Set the content to appear under the system bars so that the
                        // content doesn't resize when the system bars hide and show.
                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        // Hide the nav bar and status bar
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN);
    }
}
