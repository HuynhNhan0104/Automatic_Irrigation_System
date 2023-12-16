package com.example.iot_app;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.appcompat.app.ActionBarDrawerToggle;
import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentTransaction;

import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.example.iot_app.fragment.AccountFragment;
import com.example.iot_app.fragment.DashboardFragment;
import com.example.iot_app.fragment.DeviceFragment;
import com.example.iot_app.fragment.HomeFragment;
import com.example.iot_app.fragment.SchedulerFragment;
import com.google.android.material.navigation.NavigationView;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class ToolbarActivity extends AppCompatActivity implements NavigationView.OnNavigationItemSelectedListener{
    private DrawerLayout mDrawerLayout;
    private static final int FRAGMENT_HOME = 0;
    private static final int FRAGMENT_DASHBOARD = 1;
    private static final int FRAGMENT_DEVICEMANAGEMENT = 2;
    private static final int FRAGMENT_ACCOUNT = 3;
    private static final int FRAGMENT_SCHEDULER = 4;
    private int mCurrentFragment = FRAGMENT_HOME;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_toolbar);
        //        toolbar begin

        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);
        mDrawerLayout = findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(this,mDrawerLayout,toolbar,R.string.navigation_drawer_open,R.string.navigation_drawer_close);
        mDrawerLayout.addDrawerListener(toggle);
        toggle.syncState();
        //        toolbar end

        //Vew toolbar begin
        NavigationView navigationView = findViewById(R.id.navigation_view);
        navigationView.setNavigationItemSelectedListener(this);
        //View toolbar end.

    }


    public boolean onNavigationItemSelected(@NonNull MenuItem item) {
        int id = item.getItemId();
        if(id == R.id.nav_home){
            if(mCurrentFragment != FRAGMENT_HOME){
                replaceFragment(new HomeFragment());
                mCurrentFragment = FRAGMENT_HOME;
            }
        } else if(id == R.id.nav_dash){
            if(mCurrentFragment != FRAGMENT_DASHBOARD){
                replaceFragment(new DashboardFragment());
                mCurrentFragment = FRAGMENT_DASHBOARD;
            }
        } else if(id == R.id.nav_device){
            if(mCurrentFragment != FRAGMENT_DEVICEMANAGEMENT){
                replaceFragment(new DeviceFragment());
                mCurrentFragment = FRAGMENT_DEVICEMANAGEMENT;
            }
        } else if(id == R.id.nav_profile){
            if(mCurrentFragment != FRAGMENT_ACCOUNT){
                replaceFragment(new AccountFragment());
                mCurrentFragment = FRAGMENT_ACCOUNT;
            }
        } else if(id == R.id.nav_scheduler){
            if(mCurrentFragment != FRAGMENT_SCHEDULER){
                replaceFragment(new SchedulerFragment());
                mCurrentFragment = FRAGMENT_SCHEDULER;
            }
        }
        mDrawerLayout.closeDrawer(GravityCompat.START);
        return true;
    }

    private void replaceFragment(Fragment fragment){
        FragmentTransaction transaction = getSupportFragmentManager().beginTransaction();
        transaction.replace(R.id.content_frame,fragment);
        transaction.commit();
    }
}