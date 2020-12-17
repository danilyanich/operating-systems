package model;

public class OperatorSelection {
    public static final String SQRT = "√";
    public static final String PLUS = "+";
    public static final String MINUS = "-";
    public static final String DIVISION = "/";
    public static final String MULTIPLICATION = "*";
    public static final String MOD = "%";
    public static final String SIN = "sin";
    public static final String COS = "cos";
    public static final String TAN = "tan";
    public static final String ASIN = "asin";
    public static final String ACOS = "acos";
    public static final String ATAN = "atan";
    public static final String ATAN2 = "atan2";


    public static Operator getOperator(String operator) throws Exception {
        switch (operator) {
            case SQRT: {
                return new UnaryOperator(SQRT);
            }
            case PLUS: {
                return new BinaryOperator(PLUS);
            }
            case MINUS: {
                return new BinaryOperator(MINUS);
            }
            case DIVISION: {
                return new BinaryOperator(DIVISION);
            }
            case MULTIPLICATION: {
                return new BinaryOperator(MULTIPLICATION);
            }
            case MOD: {
                return new BinaryOperator(MOD);
            }
            case SIN: {
                return new UnaryOperator(SIN);
            }
            case COS: {
                return new UnaryOperator(COS);
            }
            case TAN: {
                return new UnaryOperator(TAN);
            }
            case ASIN: {
                return new UnaryOperator(ASIN);
            }
            case ACOS: {
                return new UnaryOperator(ACOS);
            }
            case ATAN: {
                return new UnaryOperator(ATAN);
            }
            case ATAN2: {
                return new UnaryOperator(ATAN2);
            }
        }

        throw new Exception("The token given is not an operator");
    }
}
