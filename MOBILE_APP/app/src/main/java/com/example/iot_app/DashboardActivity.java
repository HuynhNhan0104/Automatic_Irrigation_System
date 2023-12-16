package com.example.iot_app;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

public class DashboardActivity extends AppCompatActivity {
    TextView txtTemp, txtHumi;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_dashboard);
        txtTemp = findViewById(R.id.txt_soil_temp);
        txtHumi = findViewById(R.id.txt_soil_humi);
    }
}