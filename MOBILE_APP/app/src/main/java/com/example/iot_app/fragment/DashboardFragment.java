package com.example.iot_app.fragment;

import android.app.Dialog;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.TextView;
import android.util.Log;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallbackExtended;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import com.example.iot_app.R;

// for chart
import android.widget.Toast;
import com.github.mikephil.charting.data.DataSet;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.highlight.Highlight;
import android.graphics.Color;
import com.github.mikephil.charting.components.YAxis;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import com.github.mikephil.charting.charts.CombinedChart;

import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.data.CombinedData;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.listener.OnChartValueSelectedListener;
import com.github.mikephil.charting.formatter.ValueFormatter;



public class DashboardFragment extends Fragment implements OnChartValueSelectedListener {
    private Timer timer;
    private HttpHelper httpHelper;

    TextView txtTemperature;
    TextView txtHumidity;
    TextView txtLight;
    private static List<Long> timestampList = new ArrayList<>();
    private static List<Float> temperatureList = new ArrayList<>();

    private CombinedChart mChart;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.activity_dashboard, container, false);
        txtTemperature = view.findViewById(R.id.txt_soil_temp);
        txtHumidity = view.findViewById(R.id.txt_soil_humi);
        txtLight = view.findViewById(R.id.txt_lux);

        LinearLayout chart_temperature = view.findViewById(R.id.chartTemperature);
        LinearLayout temp_control = view.findViewById(R.id.temperature_control);
        chart_temperature.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                opencharttemperature(Gravity.CENTER);
            }
        });

        temp_control.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                openTemperaturecontrol(Gravity.CENTER);
            }
        });

        //http helper----------------------------------------------------------------------
        timer = new Timer();
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                // Lấy thời gian hiện tại
                Calendar endTime = Calendar.getInstance();
                Calendar startTime = (Calendar) endTime.clone();
                startTime.add(Calendar.MINUTE, -5);

                // Convert timestamps to milliseconds
                long startTs = startTime.getTimeInMillis();
                long endTs = endTime.getTimeInMillis();

                // Bearer token và các thông số cần thiết
                String thingsboardHost = "https://demo.thingsboard.io";
                int port = 80;

//                String deviceId = "9ae20180-6818-11ed-81ab-ffa1a15f3161";
//                String jtw = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJodXlxdWFuZzAwODFAZ21haWwuY29tIiwidXNlcklkIjoiY2JhMmQ0YTAtNWI1NC0xMWVkLWIyMzYtNTVhNmNkOTVjN2E2Iiwic2NvcGVzIjpbIlRFTkFOVF9BRE1JTiJdLCJzZXNzaW9uSWQiOiI5NWNlYzdkZC1iMWNlLTQ4N2ItYjBjNS1iMThlNjRjMDE4YmUiLCJpc3MiOiJ0aGluZ3Nib2FyZC5pbyIsImlhdCI6MTcwMDkxODc1NCwiZXhwIjoxNzAyNzE4NzU0LCJmaXJzdE5hbWUiOiJIYXkiLCJsYXN0TmFtZSI6IkRheSIsImVuYWJsZWQiOnRydWUsInByaXZhY3lQb2xpY3lBY2NlcHRlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6ImMwMmUzZTcwLTViNTQtMTFlZC1iMjM2LTU1YTZjZDk1YzdhNiIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAifQ.Nu_OS2mIMYs0uT3p4FDywXHgjirtLWILFg6GcCFjM9Mjyfb9ZTMiWfcycbGSC3Y7W5G0N-QtbZ1nJUgRo7YcpA";

                String deviceId = "5435e850-6c95-11ee-9242-93ef3c64501e";
                String jtw = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJoaWV1bmhhbnR2MjAwMkBnbWFpbC5jb20iLCJ1c2VySWQiOiJhNGQwYjM5MC02M2ZkLTExZWQtYTYzYS1iMWE3NzhkNWEyZDgiLCJzY29wZXMiOlsiVEVOQU5UX0FETUlOIl0sInNlc3Npb25JZCI6IjBhNWY3ZjJkLWViNTMtNDY5ZS05N2RmLWZjNjhkM2JjNGM2NyIsImlzcyI6InRoaW5nc2JvYXJkLmlvIiwiaWF0IjoxNzAwOTkwNDQ2LCJleHAiOjE3MDI3OTA0NDYsImZpcnN0TmFtZSI6Ikhp4bq_dSBOaMOibiIsImxhc3ROYW1lIjoiSHXhu7NuaCBOZ3V54buFbiIsImVuYWJsZWQiOnRydWUsInByaXZhY3lQb2xpY3lBY2NlcHRlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6ImEyNzczMzMwLTYzZmQtMTFlZC1hNjNhLWIxYTc3OGQ1YTJkOCIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAifQ.Xk0uyD3mY15ZhWKoighWVQtSSsRB31DskD0L-yZ65796JacKMFMie-uVYX07IOZC4hJvWh1stWmjmcK54hTTxA";


                // Tạo một instance của HttpHelper class
                HttpHelper httpHelper = new HttpHelper();
                httpHelper.retrieveTelemetryData(thingsboardHost, port, deviceId, jtw, startTs, endTs, new HttpHelper.DataCallback() {
                    @Override
                    public void onDataReceived(JSONObject jsonData) {
                        updateUIFromJson(jsonData);
                        try {
                            // Thay đổi từ "smoke" thành "temperature"
                            JSONArray temperatureArray = jsonData.getJSONArray("temperature");

                            for (int i = 0; i < temperatureArray.length(); i++) {
                                JSONObject temperatureObject = temperatureArray.getJSONObject(i);
                                long timestamp = temperatureObject.getLong("ts");
                                int temperatureValue = temperatureObject.getInt("value");

                                // Lưu giữ giá trị timestamp và nhiệt độ vào danh sách toàn cục
                                timestampList.add(timestamp);
                                temperatureList.add((float) temperatureValue);
                            }
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                });
            }
        }, 0, 30000);  // Lên lịch để chạy sau 0 ms, và lặp lại mỗi 60000 ms (1 phút)
        return view;
    }

    private void updateUIFromJson(JSONObject jsonData) {
        try {
            JSONArray tempArray = jsonData.getJSONArray("temperature");
            JSONArray humiArray = jsonData.getJSONArray("humidity");
            JSONArray smokeArray = jsonData.getJSONArray("lux");

            // Lấy giá trị mới nhất từ mỗi mảng
            String latestTemp = tempArray.getJSONObject(tempArray.length() - 1).getString("value");
            String latestHumi = humiArray.getJSONObject(humiArray.length() - 1).getString("value");
            String latestLight = smokeArray.getJSONObject(smokeArray.length() - 1).getString("value");

            // Cập nhật giá trị cho TextViews
            txtTemperature.setText(latestTemp);
            txtHumidity.setText(latestHumi);
            txtLight.setText(latestLight);



        } catch (JSONException e) {
            e.printStackTrace();
        }
    }



// open temperature chart------------------------------------------------------------------------
    private void opencharttemperature(int gravity){
        final Dialog dialog = new Dialog(requireContext());
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setContentView(R.layout.temperature_chart);

        mChart = (CombinedChart) dialog.findViewById(R.id.combinedChart);
        mChart.getDescription().setEnabled(false);
        mChart.setBackgroundColor(Color.WHITE);
        mChart.setDrawGridBackground(false);
        mChart.setDrawBarShadow(false);
        mChart.setHighlightFullBarEnabled(false);
        mChart.setOnChartValueSelectedListener(this);

        YAxis rightAxis = mChart.getAxisRight();
        rightAxis.setDrawGridLines(false);
        rightAxis.setAxisMinimum(0f);

        YAxis leftAxis = mChart.getAxisLeft();
        leftAxis.setDrawGridLines(false);
        leftAxis.setAxisMinimum(0f);


        XAxis xAxis = mChart.getXAxis();
        xAxis.setPosition(XAxis.XAxisPosition.BOTTOM);
        xAxis.setAxisMinimum(0f);
        xAxis.setGranularity(1f);
        xAxis.setValueFormatter(new ValueFormatter() {
            @Override
            public String getFormattedValue(float value) {
                // Sử dụng danh sách timestamp để chuyển đổi giá trị trục hoành
                int index = (int) value;
                if (index >= 0 && index < /*timestampList.size()*/ 10) {
                    long timestamp = timestampList.get(index);
                    return convertTimestampToTime(timestamp);
                }
                return "";
            }
        });

        CombinedData data = new CombinedData();
        LineData lineDatas = new LineData();
        lineDatas.addDataSet((ILineDataSet) dataChart());

        data.setData(lineDatas);

        xAxis.setAxisMaximum(data.getXMax() + 0.25f);

        mChart.setData(data);
        mChart.invalidate();


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


        dialog.show();
    }

    public void onValueSelected(Entry e, Highlight h) {
        Toast.makeText(requireContext(), "Value: " + e.getY()
                + ", index: " + h.getX() + ", DataSet index: "
                + h.getDataSetIndex(), Toast.LENGTH_SHORT).show();

    }
    public void onNothingSelected() {

    }

    private static DataSet dataChart() {

        LineData d = new LineData();
        ArrayList<Entry> entries = new ArrayList<>();

        // Sử dụng danh sách timestamp và nhiệt độ để thêm dữ liệu vào entries
        for (int i = 0; i < /*timestampList.size()*/10; i++) {
            entries.add(new Entry(i, temperatureList.get(i)));
        }

        LineDataSet set = new LineDataSet(entries, "Request Ots approved");
        set.setColor(Color.GREEN);
        set.setLineWidth(2.5f);
        set.setCircleColor(Color.GREEN);
        set.setCircleRadius(5f);
        set.setFillColor(Color.GREEN);
        set.setMode(LineDataSet.Mode.CUBIC_BEZIER);
        set.setDrawValues(true);
        set.setValueTextSize(10f);
        set.setValueTextColor(Color.GREEN);

        set.setAxisDependency(YAxis.AxisDependency.LEFT);
        d.addDataSet(set);

        return set;
    }

    //----------------------------------------------------------------------------------------------------
    private void openTemperaturecontrol(int gravity){
        final Dialog dialog = new Dialog(requireContext());
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setContentView(R.layout.temperature_control);

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
        LinearLayout bgTempControl;
        tv = dialog.findViewById(R.id.status_temp_seekbar);
        sbar = dialog.findViewById(R.id.seekBar_temp);
        picture = dialog.findViewById(R.id.temp_picture);
        bgTempControl = dialog.findViewById(R.id.bg_temp_control);

        sbar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                tv.setText(String.valueOf(i));
                if( i > 30){
                    picture.setImageResource(R.drawable.hot_icon);
                    bgTempControl.setBackgroundResource(R.drawable.bg_windows_red);
                } else if(i <=20 && i >10){
                    picture.setImageResource(R.drawable.cool_icon);
                    bgTempControl.setBackgroundResource(R.drawable.bg_windows);
                } else if(i <= 10){
                    picture.setImageResource(R.drawable.cold_icon);
                    bgTempControl.setBackgroundResource(R.drawable.bg_windows_blue);
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
    private String convertTimestampToTime(long timestamp) {
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm", Locale.getDefault());
        return sdf.format(new Date(timestamp));
    }

}
