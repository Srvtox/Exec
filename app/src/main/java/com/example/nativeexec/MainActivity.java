package com.example.nativeexec;

import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {
    
    private NativeExec nativeExec;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_simple);

        nativeExec = new NativeExec();

        EditText input = findViewById(R.id.input_cmd);
        Button runBtn = findViewById(R.id.run_btn);
        TextView output = findViewById(R.id.output_tv);

        runBtn.setOnClickListener(v -> {
            String cmd = input.getText().toString();
            NativeExec.ExecResult res = nativeExec.exec(cmd);
            output.setText("Exit: " + res.exitCode + "\nOutput:\n" + res.output);
        });
    }
}
