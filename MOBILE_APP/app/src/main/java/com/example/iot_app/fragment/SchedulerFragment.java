
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

public class SchedulerFragment extends Fragment {
    @Nullable
    MQTTHelper_Device mqttHelper;
    private HttpHelper httpHelper;
    private boolean isPowerOn = true;
    private Calendar selectedDateTime = Calendar.getInstance();
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_scheduler, container, false);
        LinearLayout lightControl = view.findViewById(R.id.light_control_scheduler);
        LinearLayout pumpControl = view.findViewById(R.id.pump_control_scheduler);
        LinearLayout fanControl = view.findViewById(R.id.fan_control_scheduler);
        lightControl.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Xử lý sự kiện khi LinearLayout được click
                showDatePicker_led();
            }
        });

        pumpControl.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Xử lý sự kiện khi LinearLayout được click
                showDatePicker_pump();
            }
        });
        fanControl.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // Xử lý sự kiện khi LinearLayout được click
                showDatePicker_fan();
            }
        });

        return view;
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


    //led- scheduler-----------------------------------------------
    private void showDatePicker_led() {
        DatePickerDialog datePickerDialog = new DatePickerDialog(
                requireContext(),
                new DatePickerDialog.OnDateSetListener() {
                    @Override
                    public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
                        selectedDateTime.set(Calendar.YEAR, year);
                        selectedDateTime.set(Calendar.MONTH, month);
                        selectedDateTime.set(Calendar.DAY_OF_MONTH, dayOfMonth);

                        showTimePicker_led();
                    }
                },
                selectedDateTime.get(Calendar.YEAR),
                selectedDateTime.get(Calendar.MONTH),
                selectedDateTime.get(Calendar.DAY_OF_MONTH)
        );

        datePickerDialog.show();
    }

    private void showTimePicker_led() {
        TimePickerDialog timePickerDialog = new TimePickerDialog(
                requireContext(),
                new TimePickerDialog.OnTimeSetListener() {
                    @Override
                    public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
                        selectedDateTime.set(Calendar.HOUR_OF_DAY, hourOfDay);
                        selectedDateTime.set(Calendar.MINUTE, minute);

                        // Log giá trị đã pick
                        logSelectedDateTime_led();
                    }
                },
                selectedDateTime.get(Calendar.HOUR_OF_DAY),
                selectedDateTime.get(Calendar.MINUTE),
                true // 24-hour format
        );

        timePickerDialog.show();
    }

    private void logSelectedDateTime_led() {
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm", Locale.getDefault());
        String formattedTime = sdf.format(selectedDateTime.getTime());
        Log.d("SelectedTime", formattedTime);

        // Tìm TextView theo ID
        TextView lightSchedulerStatusTextView = getView().findViewById(R.id.light_scheduler_status);

        // Kiểm tra xem TextView có tồn tại hay không
        if (lightSchedulerStatusTextView != null) {
            // Đặt nội dung của TextView thành giá trị đã chọn
            lightSchedulerStatusTextView.setText(formattedTime);
        }
        String thingsboardHost = "https://demo.thingsboard.io";
        String accessToken = "5435e850-6c95-11ee-9242-93ef3c64501e";
        String jtw = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJoaWV1bmhhbnR2MjAwMkBnbWFpbC5jb20iLCJ1c2VySWQiOiJhNGQwYjM5MC02M2ZkLTExZWQtYTYzYS1iMWE3NzhkNWEyZDgiLCJzY29wZXMiOlsiVEVOQU5UX0FETUlOIl0sInNlc3Npb25JZCI6IjBhNWY3ZjJkLWViNTMtNDY5ZS05N2RmLWZjNjhkM2JjNGM2NyIsImlzcyI6InRoaW5nc2JvYXJkLmlvIiwiaWF0IjoxNzAwOTkwNDQ2LCJleHAiOjE3MDI3OTA0NDYsImZpcnN0TmFtZSI6Ikhp4bq_dSBOaMOibiIsImxhc3ROYW1lIjoiSHXhu7NuaCBOZ3V54buFbiIsImVuYWJsZWQiOnRydWUsInByaXZhY3lQb2xpY3lBY2NlcHRlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6ImEyNzczMzMwLTYzZmQtMTFlZC1hNjNhLWIxYTc3OGQ1YTJkOCIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAifQ.Xk0uyD3mY15ZhWKoighWVQtSSsRB31DskD0L-yZ65796JacKMFMie-uVYX07IOZC4hJvWh1stWmjmcK54hTTxA";

        httpHelper = new HttpHelper();
        httpHelper.sendRPCCall(thingsboardHost, accessToken,"1", jtw, "led_scheduler", formattedTime, new HttpHelper.DataCallback() {
            @Override
            public void onDataReceived(JSONObject jsonData) {
                // Xử lý dữ liệu phản hồi từ cuộc gọi RPC (nếu cần)
            }
        });
    }

    //pump - scheduler-----------------------------------------------
    private void showDatePicker_pump() {
        DatePickerDialog datePickerDialog = new DatePickerDialog(
                requireContext(),
                new DatePickerDialog.OnDateSetListener() {
                    @Override
                    public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
                        selectedDateTime.set(Calendar.YEAR, year);
                        selectedDateTime.set(Calendar.MONTH, month);
                        selectedDateTime.set(Calendar.DAY_OF_MONTH, dayOfMonth);

                        showTimePicker_pump();
                    }
                },
                selectedDateTime.get(Calendar.YEAR),
                selectedDateTime.get(Calendar.MONTH),
                selectedDateTime.get(Calendar.DAY_OF_MONTH)
        );

        datePickerDialog.show();
    }

    private void showTimePicker_pump() {
        TimePickerDialog timePickerDialog = new TimePickerDialog(
                requireContext(),
                new TimePickerDialog.OnTimeSetListener() {
                    @Override
                    public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
                        selectedDateTime.set(Calendar.HOUR_OF_DAY, hourOfDay);
                        selectedDateTime.set(Calendar.MINUTE, minute);

                        // Log giá trị đã pick
                        logSelectedDateTime_pump();
                    }
                },
                selectedDateTime.get(Calendar.HOUR_OF_DAY),
                selectedDateTime.get(Calendar.MINUTE),
                true // 24-hour format
        );

        timePickerDialog.show();
    }

    private void logSelectedDateTime_pump() {
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm", Locale.getDefault());
        String formattedTime = sdf.format(selectedDateTime.getTime());
        Log.d("SelectedTime", formattedTime);

        // Tìm TextView theo ID
        TextView lightSchedulerStatusTextView = getView().findViewById(R.id.pump_scheduler_status);

        // Kiểm tra xem TextView có tồn tại hay không
        if (lightSchedulerStatusTextView != null) {
            // Đặt nội dung của TextView thành giá trị đã chọn
            lightSchedulerStatusTextView.setText(formattedTime);
        }
        String thingsboardHost = "https://demo.thingsboard.io";
        String accessToken = "5435e850-6c95-11ee-9242-93ef3c64501e";
        String jtw = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJoaWV1bmhhbnR2MjAwMkBnbWFpbC5jb20iLCJ1c2VySWQiOiJhNGQwYjM5MC02M2ZkLTExZWQtYTYzYS1iMWE3NzhkNWEyZDgiLCJzY29wZXMiOlsiVEVOQU5UX0FETUlOIl0sInNlc3Npb25JZCI6IjBhNWY3ZjJkLWViNTMtNDY5ZS05N2RmLWZjNjhkM2JjNGM2NyIsImlzcyI6InRoaW5nc2JvYXJkLmlvIiwiaWF0IjoxNzAwOTkwNDQ2LCJleHAiOjE3MDI3OTA0NDYsImZpcnN0TmFtZSI6Ikhp4bq_dSBOaMOibiIsImxhc3ROYW1lIjoiSHXhu7NuaCBOZ3V54buFbiIsImVuYWJsZWQiOnRydWUsInByaXZhY3lQb2xpY3lBY2NlcHRlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6ImEyNzczMzMwLTYzZmQtMTFlZC1hNjNhLWIxYTc3OGQ1YTJkOCIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAifQ.Xk0uyD3mY15ZhWKoighWVQtSSsRB31DskD0L-yZ65796JacKMFMie-uVYX07IOZC4hJvWh1stWmjmcK54hTTxA";

        httpHelper = new HttpHelper();
        httpHelper.sendRPCCall(thingsboardHost, accessToken,"1", jtw, "pump_scheduler", formattedTime, new HttpHelper.DataCallback() {
            @Override
            public void onDataReceived(JSONObject jsonData) {
                // Xử lý dữ liệu phản hồi từ cuộc gọi RPC (nếu cần)
            }
        });
    }

    //fan - scheduler-----------------------------------------------
    private void showDatePicker_fan() {
        DatePickerDialog datePickerDialog = new DatePickerDialog(
                requireContext(),
                new DatePickerDialog.OnDateSetListener() {
                    @Override
                    public void onDateSet(DatePicker view, int year, int month, int dayOfMonth) {
                        selectedDateTime.set(Calendar.YEAR, year);
                        selectedDateTime.set(Calendar.MONTH, month);
                        selectedDateTime.set(Calendar.DAY_OF_MONTH, dayOfMonth);

                        showTimePicker_fan();
                    }
                },
                selectedDateTime.get(Calendar.YEAR),
                selectedDateTime.get(Calendar.MONTH),
                selectedDateTime.get(Calendar.DAY_OF_MONTH)
        );

        datePickerDialog.show();
    }

    private void showTimePicker_fan() {
        TimePickerDialog timePickerDialog = new TimePickerDialog(
                requireContext(),
                new TimePickerDialog.OnTimeSetListener() {
                    @Override
                    public void onTimeSet(TimePicker view, int hourOfDay, int minute) {
                        selectedDateTime.set(Calendar.HOUR_OF_DAY, hourOfDay);
                        selectedDateTime.set(Calendar.MINUTE, minute);

                        // Log giá trị đã pick
                        logSelectedDateTime_fan();
                    }
                },
                selectedDateTime.get(Calendar.HOUR_OF_DAY),
                selectedDateTime.get(Calendar.MINUTE),
                true // 24-hour format
        );

        timePickerDialog.show();
    }

    private void logSelectedDateTime_fan() {
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm", Locale.getDefault());
        String formattedTime = sdf.format(selectedDateTime.getTime());
        Log.d("SelectedTime", formattedTime);

        // Tìm TextView theo ID
        TextView lightSchedulerStatusTextView = getView().findViewById(R.id.fan_scheduler_status);

        // Kiểm tra xem TextView có tồn tại hay không
        if (lightSchedulerStatusTextView != null) {
            // Đặt nội dung của TextView thành giá trị đã chọn
            lightSchedulerStatusTextView.setText(formattedTime);
        }
        String thingsboardHost = "https://demo.thingsboard.io";
        String accessToken = "5435e850-6c95-11ee-9242-93ef3c64501e";
        String jtw = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJoaWV1bmhhbnR2MjAwMkBnbWFpbC5jb20iLCJ1c2VySWQiOiJhNGQwYjM5MC02M2ZkLTExZWQtYTYzYS1iMWE3NzhkNWEyZDgiLCJzY29wZXMiOlsiVEVOQU5UX0FETUlOIl0sInNlc3Npb25JZCI6IjBhNWY3ZjJkLWViNTMtNDY5ZS05N2RmLWZjNjhkM2JjNGM2NyIsImlzcyI6InRoaW5nc2JvYXJkLmlvIiwiaWF0IjoxNzAwOTkwNDQ2LCJleHAiOjE3MDI3OTA0NDYsImZpcnN0TmFtZSI6Ikhp4bq_dSBOaMOibiIsImxhc3ROYW1lIjoiSHXhu7NuaCBOZ3V54buFbiIsImVuYWJsZWQiOnRydWUsInByaXZhY3lQb2xpY3lBY2NlcHRlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6ImEyNzczMzMwLTYzZmQtMTFlZC1hNjNhLWIxYTc3OGQ1YTJkOCIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAifQ.Xk0uyD3mY15ZhWKoighWVQtSSsRB31DskD0L-yZ65796JacKMFMie-uVYX07IOZC4hJvWh1stWmjmcK54hTTxA";

        httpHelper = new HttpHelper();
        httpHelper.sendRPCCall(thingsboardHost, accessToken,"1", jtw, "fan_scheduler", formattedTime, new HttpHelper.DataCallback() {
            @Override
            public void onDataReceived(JSONObject jsonData) {
                // Xử lý dữ liệu phản hồi từ cuộc gọi RPC (nếu cần)
            }
        });
    }
}
