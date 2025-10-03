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
    
}