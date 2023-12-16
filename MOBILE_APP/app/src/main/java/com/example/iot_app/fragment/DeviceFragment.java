package com.example.iot_app.fragment;

import android.app.DatePickerDialog;
import android.app.Dialog;
import android.app.TimePickerDialog;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.DatePicker;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.ImageView;
import androidx.cardview.widget.CardView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.nio.charset.Charset;

import com.example.iot_app.R;


import android.content.Context;
import android.util.Log;
import android.widget.TimePicker;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Locale;

public class DeviceFragment extends Fragment {
    @Nullable
    MQTTHelper_Device mqttHelper;
    private HttpHelper httpHelper;
    private boolean isPowerOn = true;
    private Calendar selectedDateTime = Calendar.getInstance();
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.device_management, container, false);
        LinearLayout lightControl = view.findViewById(R.id.light_control);
        //http--------------------------------------------------------------------------------
        String thingsboardHost = "https://demo.thingsboard.io";

//        String accessToken = "9ae20180-6818-11ed-81ab-ffa1a15f3161";
//        String jtw = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJodXlxdWFuZzAwODFAZ21haWwuY29tIiwidXNlcklkIjoiY2JhMmQ0YTAtNWI1NC0xMWVkLWIyMzYtNTVhNmNkOTVjN2E2Iiwic2NvcGVzIjpbIlRFTkFOVF9BRE1JTiJdLCJzZXNzaW9uSWQiOiI5NWNlYzdkZC1iMWNlLTQ4N2ItYjBjNS1iMThlNjRjMDE4YmUiLCJpc3MiOiJ0aGluZ3Nib2FyZC5pbyIsImlhdCI6MTcwMDkxODc1NCwiZXhwIjoxNzAyNzE4NzU0LCJmaXJzdE5hbWUiOiJIYXkiLCJsYXN0TmFtZSI6IkRheSIsImVuYWJsZWQiOnRydWUsInByaXZhY3lQb2xpY3lBY2NlcHRlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6ImMwMmUzZTcwLTViNTQtMTFlZC1iMjM2LTU1YTZjZDk1YzdhNiIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAifQ.Nu_OS2mIMYs0uT3p4FDywXHgjirtLWILFg6GcCFjM9Mjyfb9ZTMiWfcycbGSC3Y7W5G0N-QtbZ1nJUgRo7YcpA";

        String accessToken = "5435e850-6c95-11ee-9242-93ef3c64501e";
        String jtw = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJoaWV1bmhhbnR2MjAwMkBnbWFpbC5jb20iLCJ1c2VySWQiOiJhNGQwYjM5MC02M2ZkLTExZWQtYTYzYS1iMWE3NzhkNWEyZDgiLCJzY29wZXMiOlsiVEVOQU5UX0FETUlOIl0sInNlc3Npb25JZCI6IjBhNWY3ZjJkLWViNTMtNDY5ZS05N2RmLWZjNjhkM2JjNGM2NyIsImlzcyI6InRoaW5nc2JvYXJkLmlvIiwiaWF0IjoxNzAwOTkwNDQ2LCJleHAiOjE3MDI3OTA0NDYsImZpcnN0TmFtZSI6Ikhp4bq_dSBOaMOibiIsImxhc3ROYW1lIjoiSHXhu7NuaCBOZ3V54buFbiIsImVuYWJsZWQiOnRydWUsInByaXZhY3lQb2xpY3lBY2NlcHRlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6ImEyNzczMzMwLTYzZmQtMTFlZC1hNjNhLWIxYTc3OGQ1YTJkOCIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAifQ.Xk0uyD3mY15ZhWKoighWVQtSSsRB31DskD0L-yZ65796JacKMFMie-uVYX07IOZC4hJvWh1stWmjmcK54hTTxA";

        httpHelper = new HttpHelper();

        //button-----------------------------------------------------------------------------
        CardView buttonLight = view.findViewById(R.id.button_light);
        CardView buttonPumb = view.findViewById(R.id.button_pumb);
        CardView buttonFan = view.findViewById(R.id.button_fan);
        buttonLight.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                isPowerOn = !isPowerOn;
                if (isPowerOn) {
                    buttonLight.setCardBackgroundColor(Color.parseColor("#00CC66")); // Màu nền khi bật
                        httpHelper.sendRPCCall(thingsboardHost, accessToken,"1", jtw, "led", "1", new HttpHelper.DataCallback() {
                            @Override
                            public void onDataReceived(JSONObject jsonData) {
                                // Xử lý dữ liệu phản hồi từ cuộc gọi RPC (nếu cần)
                            }
                        });

                } else {
                    buttonLight.setCardBackgroundColor(Color.parseColor("#F44336")); // Màu nền khi tắt
                    httpHelper.sendRPCCall(thingsboardHost, accessToken,"1", jtw, "led", "0", new HttpHelper.DataCallback() {
                        @Override
                        public void onDataReceived(JSONObject jsonData) {
                            // Xử lý dữ liệu phản hồi từ cuộc gọi RPC (nếu cần)
                        }
                    });
                }
            }
        });
        buttonPumb.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                isPowerOn = !isPowerOn;
                if (isPowerOn) {
                    buttonPumb.setCardBackgroundColor(Color.parseColor("#00CC66")); // Màu nền khi bật
                    httpHelper.sendRPCCall(thingsboardHost, accessToken,"1", jtw, "pump", "1", new HttpHelper.DataCallback() {
                        @Override
                        public void onDataReceived(JSONObject jsonData) {
                            // Xử lý dữ liệu phản hồi từ cuộc gọi RPC (nếu cần)
                        }
                    });
                } else {
                    buttonPumb.setCardBackgroundColor(Color.parseColor("#F44336")); // Màu nền khi tắt
                    httpHelper.sendRPCCall(thingsboardHost, accessToken,"1", jtw, "pump", "0", new HttpHelper.DataCallback() {
                        @Override
                        public void onDataReceived(JSONObject jsonData) {
                            // Xử lý dữ liệu phản hồi từ cuộc gọi RPC (nếu cần)
                        }
                    });
                }
            }
        });
        buttonFan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                isPowerOn = !isPowerOn;
                if (isPowerOn) {
                    buttonFan.setCardBackgroundColor(Color.parseColor("#00CC66")); // Màu nền khi bật
                } else {
                    buttonFan.setCardBackgroundColor(Color.parseColor("#F44336")); // Màu nền khi tắt
                }
            }
        });
        //button-----------

        //startMQTT();
        lightControl.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Xử lý sự kiện khi LinearLayout được click
                openLightcontrol(Gravity.CENTER);
            }
        });




        return view;
    }

    private void openLightcontrol(int gravity){
        final Dialog dialog = new Dialog(requireContext());
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setContentView(R.layout.control_light_layout);

        Window window = dialog.getWindow();
        if(window == null){
            return;
        }
        window.setLayout(WindowManager.LayoutParams.MATCH_PARENT, WindowManager.LayoutParams.WRAP_CONTENT);
        window.setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));

        WindowManager.LayoutParams windowAttributes = window.getAttributes();
        windowAttributes.gravity = gravity;
        window.setAttributes(windowAttributes);


        if(Gravity.CENTER == gravity){
            dialog.setCancelable(true);
        } else{
            dialog.setCancelable(false);
        }

//        Hiển thị thông số seekbar lên texview
        TextView tv;
        SeekBar sbar;
        ImageView picture;
        tv = dialog.findViewById(R.id.status_light_seekbar);
        sbar = dialog.findViewById(R.id.seekBar_light);
        picture = dialog.findViewById(R.id.light_picture);

        sbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                tv.setText(String.valueOf(i));
                if( i > 20){
                    picture.setImageResource(R.drawable.light_turnon_image);
                } else{
                    picture.setImageResource(R.drawable.light_image);
                }
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
//------------------------------------


        dialog.show();
    }

    public void startMQTT(){
        mqttHelper = new MQTTHelper_Device(this);
        mqttHelper.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean reconnect, String serverURI) {

            }

            @Override
            public void connectionLost(Throwable cause) {

            }

            @Override
            public void messageArrived(String topic, MqttMessage message) throws Exception {

            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {

            }
        });
    }


    public void sendDataMQTT(String key, String value) {
        try {
            JSONObject telemetryData = new JSONObject();
            telemetryData.put(key, value);

            MqttMessage msg = new MqttMessage();
            msg.setId(1234);
            msg.setQos(0);
            msg.setRetained(false);

            byte[] b = telemetryData.toString().getBytes(Charset.forName("UTF-8"));
            msg.setPayload(b);

            mqttHelper.mqttAndroidClient.publish("v1/devices/me/telemetry", msg);//v1/devices/me/telemetry v1/devices/me/rpc/request/+

        } catch (JSONException | MqttException e) {
            e.printStackTrace();
        }
    }


    //scheduler-----------------------------------------------
    private void showDatePicker() {
        DatePickerDialog datePickerDialog = new DatePickerDialog(
                requireContext(),
                new DatePickerDialog.OnDateSetListener() {
                    @Override
                    public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
                        selectedDateTime.set(Calendar.YEAR, year);
                        selectedDateTime.set(Calendar.MONTH, month);
                        selectedDateTime.set(Calendar.DAY_OF_MONTH, dayOfMonth);

                        showTimePicker();
                    }
                },
                selectedDateTime.get(Calendar.YEAR),
                selectedDateTime.get(Calendar.MONTH),
                selectedDateTime.get(Calendar.DAY_OF_MONTH)
        );

        datePickerDialog.show();
    }

    private void showTimePicker() {
        TimePickerDialog timePickerDialog = new TimePickerDialog(
                requireContext(),
                new TimePickerDialog.OnTimeSetListener() {
                    @Override
                    public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
                        selectedDateTime.set(Calendar.HOUR_OF_DAY, hourOfDay);
                        selectedDateTime.set(Calendar.MINUTE, minute);

                        // Log giá trị đã pick
                        logSelectedDateTime();
                    }
                },
                selectedDateTime.get(Calendar.HOUR_OF_DAY),
                selectedDateTime.get(Calendar.MINUTE),
                true // 24-hour format
        );

        timePickerDialog.show();
    }

    private void logSelectedDateTime() {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm", Locale.getDefault());
        String formattedDateTime = sdf.format(selectedDateTime.getTime());
        Log.d("SelectedDateTime", formattedDateTime);
    }

}
