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
            String cmd = input.getText().toString().trim();
            if (cmd.isEmpty()) {
                output.setText("لطفاً یک دستور وارد کنید");
                return;
            }

            String[] parts = cmd.split(" ");
            String command = parts[0];
            String[] args = null;
            if (parts.length > 1) {
                args = new String[parts.length - 1];
                System.arraycopy(parts, 1, args, 0, args.length);
            }

            ExecResult res = nativeExec.nativeExecCommand(command, args, null, "/");
            output.setText("Exit: " + res.exitCode + "\nOutput:\n" + res.output);
        });
    }
}