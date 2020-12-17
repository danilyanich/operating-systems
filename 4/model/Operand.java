package model;


public class Operand implements Source {
    private double value;
    private ButtonsName buttonsName;

    public Operand(String source) throws NumberFormatException {
        buttonsName = new ButtonsName(source);

        value = Double.parseDouble(source);
    }

    public Operand(double value) {
        this.value = value;
        buttonsName = new ButtonsName(String.valueOf(this.value));
    }

    public double value() {
        return value;
    }

    public ButtonsName buttonsName() {
        return buttonsName;
    }

    @Override
    public String getSource() {
        return buttonsName.source();
    }
}
