package com.example.nativeexec;

public class NativeExec {

    static {
        System.loadLibrary("nativeexec");
    }

    public native ExecResult nativeExecCommand(
    String command,
    String[] args,
    String[] env,
    String cwd
);

    public ExecResult exec(String command, String[] args, String[] env, String cwd) {
        return nativeExecCommand(command, args, env, cwd);
    }

    public static class ExecResult {
    public int exitCode;
    public String output;

    public ExecResult(int code, String out) {
        this.exitCode = code;
        this.output = out;
    }
}
}