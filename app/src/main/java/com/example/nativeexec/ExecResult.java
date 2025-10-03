package com.example.nativeexec;

public static class ExecResult {
    public int exitCode;
    public String output;

    public ExecResult(int code, String out) {
        this.exitCode = code;
        this.output = out;
    }
}