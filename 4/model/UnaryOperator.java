package model;


public class UnaryOperator implements Operator, Source {
    private Operand operand;
    private ButtonsName buttonsName;


    public UnaryOperator(String source) {
        buttonsName = new ButtonsName(source);
    }

    @Override
    public Operand result() {
        switch (buttonsName.source()) {
            case OperatorSelection.SQRT: {
                return new Operand(Math.sqrt(operand.value()));
            }

            case OperatorSelection.SIN: {
                return new Operand(Math.sin(operand.value()));
            }
            case OperatorSelection.COS: {
                return new Operand(Math.cos(operand.value()));
            }
            case OperatorSelection.TAN: {
                return new Operand(Math.tan(operand.value()));
            }
            case OperatorSelection.ASIN: {
                return new Operand(Math.asin(operand.value()));
            }
            case OperatorSelection.ACOS: {
                return new Operand(Math.acos(operand.value()));
            }
            case OperatorSelection.ATAN: {
                return new Operand(Math.atan(operand.value()));
            }
            case OperatorSelection.ATAN2: {
                return new Operand(Math.atan2(operand.value(),operand.value()));
            }
        }

        return null;
    }

    public void setOperand(Operand operand) {
        this.operand = operand;
    }

    public ButtonsName getButtonsName() {
        return buttonsName;
    }

    @Override
    public String getSource() {
        return buttonsName.source();
    }
}
