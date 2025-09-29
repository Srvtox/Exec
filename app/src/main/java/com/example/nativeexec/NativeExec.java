package com.example.nativeexec;

public class NativeExec {
    public static class ExecResult {
        public int exitCode;
        public String output;
    }

    private native ExecResult nativeExecCommand(String cmd);

    public ExecResult exec(String cmd) {
        return nativeExecCommand(cmd);
    }
}
