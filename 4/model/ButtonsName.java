package model;

public class ButtonsName {
    private String source;

    public ButtonsName(String source) {

        this.source = source;
    }


    public String source() {
        return source;
    }

    @Override
    public String toString() {
        return source;
    }
}
