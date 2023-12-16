package com.example.iot_app.fragment;

import android.os.AsyncTask;
import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;

import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class HttpHelper {
    private static final String TAG = "HttpHelper";

    // Interface để callback dữ liệu về Fragment
    public interface DataCallback {
        void onDataReceived(JSONObject jsonData);
    }

    public void retrieveTelemetryData(String thingsboardHost, int port, String deviceId, String jtw, long startTs, long endTs, DataCallback callback) {
        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... voids) {
                String apiGetTemp = thingsboardHost + "/api/plugins/telemetry/DEVICE/" + deviceId +
                        "/values/timeseries?keys=temperature,humidity,lux&startTs=" + startTs + "&endTs=" + endTs + "&interval=60000";

                try {
                    URL url = new URL(apiGetTemp);
                    HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                    connection.setRequestMethod("GET");
                    connection.setRequestProperty("Content-Type", "application/json");
                    connection.setRequestProperty("X-Authorization", jtw);

                    int responseCode = connection.getResponseCode();

                    if (responseCode == 200) {
                        Scanner scanner = new Scanner(connection.getInputStream());
                        StringBuilder response = new StringBuilder();
                        while (scanner.hasNext()) {
                            response.append(scanner.next());
                        }
                        scanner.close();

                        String jsonResponse = response.toString();
                        Log.d(TAG, "JSON Response: " + jsonResponse);

                        try {
                            // Chuyển đổi chuỗi JSON thành đối tượng JSONObject
                            JSONObject jsonData = new JSONObject(jsonResponse);

                            // Gọi callback để truyền dữ liệu JSON cho Fragment
                            if (callback != null) {
                                callback.onDataReceived(jsonData);
                            }

                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    } else {
                        Log.d(TAG, "Failed to retrieve telemetry data. Status code: " + responseCode);
                    }

                    connection.disconnect();
                } catch (IOException e) {
                    e.printStackTrace();
                }

                return null;
            }
        }.execute();  // Execute AsyncTask
    }


    public void sendRPCCall(String thingsboardHost, String accessToken, String deviceId, String jtw, String method, String params, DataCallback callback) {
        new AsyncTask<Void, Void, Void>() {
            @Override
            protected Void doInBackground(Void... voids) {
                try {
                    // Xây dựng URL cho cuộc gọi RPC
                    String rpcUrl = thingsboardHost + "/api/rpc/oneway/" + accessToken;
                    // Tạo một kết nối HTTP
                    URL url = new URL(rpcUrl);
                    HttpURLConnection connection = (HttpURLConnection) url.openConnection();
                    connection.setRequestMethod("POST");
                    connection.setRequestProperty("Content-Type", "application/json");
                    connection.setRequestProperty("X-Authorization", jtw);
                    connection.setDoOutput(true);

                    // Tạo JSON object chứa thông tin cuộc gọi RPC
                    JSONObject rpcRequest = new JSONObject();
                    rpcRequest.put("method", method);
                    rpcRequest.put("params", params);

                    // Ghi dữ liệu JSON vào luồng đầu ra của kết nối
                    connection.getOutputStream().write(rpcRequest.toString().getBytes());

                    // Lấy mã phản hồi từ kết nối
                    int responseCode = connection.getResponseCode();

                    if (responseCode == 200) {
                        // Đọc dữ liệu đáp ứng nếu cần thiết
                        InputStream inputStream = connection.getInputStream();
                        if (inputStream != null) {
                            Scanner scanner = new Scanner(inputStream);
                            StringBuilder response = new StringBuilder();
                            while (scanner.hasNext()) {
                                response.append(scanner.next());
                            }
                            scanner.close();

                            String jsonResponse = response.toString();
                            Log.d(TAG, "JSON Response: " + jsonResponse);

                            // Kiểm tra xem dữ liệu JSON có rỗng không
                            if (!jsonResponse.isEmpty()) {
                                try {
                                    // Chuyển đổi chuỗi JSON thành đối tượng JSONObject
                                    JSONObject jsonData = new JSONObject(jsonResponse);

                                    // Gọi callback để truyền dữ liệu JSON
                                    if (callback != null) {
                                        callback.onDataReceived(jsonData);
                                    }
                                } catch (JSONException e) {
                                    e.printStackTrace();
                                }
                            } else {
                                Log.d(TAG, "No data to read from InputStream.");
                            }
                        } else {
                            Log.d(TAG, "No data to read from InputStream.");
                        }
                    } else {
                        Log.d(TAG, "Failed to send RPC request. Status code: " + responseCode);

                        // Đọc dữ liệu từ ErrorStream nếu có
                        InputStream errorStream = connection.getErrorStream();
                        if (errorStream != null) {
                            Scanner scanner = new Scanner(errorStream);
                            StringBuilder errorResponse = new StringBuilder();
                            while (scanner.hasNext()) {
                                errorResponse.append(scanner.next());
                            }
                            scanner.close();

                            String errorJsonResponse = errorResponse.toString();
                            Log.d(TAG, "Error JSON Response: " + errorJsonResponse);
                        }
                    }

                    // Đóng kết nối
                    connection.disconnect();
                } catch (IOException | JSONException e) {
                    e.printStackTrace();
                }

                return null;
            }
        }.execute();
    }

}