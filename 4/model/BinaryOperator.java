package model;


public class BinaryOperator implements Operator, Source {
    private Operand leftOperand;
    private Operand rightOperand;
    private ButtonsName buttonsName;


    public BinaryOperator(String source) {
        buttonsName = new ButtonsName(source);
    }

    @Override
    public Operand result() {
        switch (buttonsName.source()) {
            case OperatorSelection.PLUS: {
                return new Operand(rightOperand.value() + leftOperand.value());
            }
            case OperatorSelection.MINUS: {
                return new Operand(rightOperand.value() - leftOperand.value());
            }
            case OperatorSelection.DIVISION: {
                return new Operand(rightOperand.value() / leftOperand.value());
            }
            case OperatorSelection.MULTIPLICATION: {
                return new Operand(rightOperand.value() * leftOperand.value());
            }
            case OperatorSelection.MOD: {
                return new Operand(rightOperand.value() % leftOperand.value());
            }
        }

        return new Operand(0);
    }

    public void setLeftOperand(Operand operand) {
        leftOperand = operand;
    }

    public void setRightOperand(Operand operand) {
        rightOperand = operand;
    }

    public ButtonsName getButtonsName() {
        return buttonsName;
    }

    @Override
    public String getSource() {
        return buttonsName.source();
    }
}
