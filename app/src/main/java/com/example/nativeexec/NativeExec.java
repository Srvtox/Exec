public class NativeExec {

    static {
        System.loadLibrary("nativeexec");
    }

    public native ExecResult nativeExecCommand(String command);

    public ExecResult exec(String cmd) {
        return nativeExecCommand(cmd);
    }

    public static class ExecResult {
        public int exitCode;
        public String output;
    }
}