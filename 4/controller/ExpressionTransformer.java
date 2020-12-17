package controller;

import model.*;
import view.ButtonPanel;
import java.util.*;

public class ExpressionTransformer {
    public static final String DOT = ".";
    private static final String EXPRESSION_BORDER = "#";

    private String source;
    private Expression expression;

    private Deque<String> operatorStack;


    public ExpressionTransformer(String source) {
        this.source = source;
        expression = new Expression();

        operatorStack = new ArrayDeque<>();
    }

    public Expression convert() throws Exception {
        operatorStack.push(EXPRESSION_BORDER);
        source = source.concat(EXPRESSION_BORDER);
        String currentOperand = "";
        String currentOperationSymbol;

        for (int expIndex = 0; expIndex < source.length(); expIndex++) {
            if (isOperand(source.substring(expIndex, expIndex + 1))) {
                currentOperand = currentOperand.concat(source.substring(expIndex, expIndex + 1));
            } else {
                if (!currentOperand.equals("")) {
                    expression.addButtonName(new Operand(currentOperand).buttonsName());
                    currentOperand = "";
                }

                currentOperationSymbol = source.substring(expIndex, expIndex + 1);


                if (source.substring(expIndex, expIndex + ButtonPanel.OPEN.length()).equals(ButtonPanel.OPEN))
                {
                    expIndex = addOpenBracket(expIndex);
                    continue;
                }


                if (currentOperationSymbol.equals(OperatorSelection.MINUS)||currentOperationSymbol.equals(OperatorSelection.PLUS))
                {
                    expIndex = addPlusMinusOperator(expIndex);
                    continue;
                }


                if (currentOperationSymbol.equals(OperatorSelection.DIVISION)
                        || currentOperationSymbol.equals(OperatorSelection.MULTIPLICATION)
                        || currentOperationSymbol.equals(OperatorSelection.MOD))
                {
                    addMultDivModOperator(expIndex);
                    continue;
                }


                if (source.substring(expIndex, expIndex + ButtonPanel.CLOSE.length()).equals(ButtonPanel.CLOSE))
                {
                    addCloseBracket();
                    continue;
                }


                if (source.substring(expIndex, expIndex + EXPRESSION_BORDER.length()).equals(EXPRESSION_BORDER))
                {
                    endParse();
                    continue;
                }


                if (source.substring(expIndex, expIndex + OperatorSelection.SIN.length()).equals(OperatorSelection.SIN))
                {
                    operatorStack.push(OperatorSelection.SIN);
                    expIndex += OperatorSelection.SIN.length() - 1;
                    continue;
                }

                if (source.substring(expIndex, expIndex + OperatorSelection.COS.length()).equals(OperatorSelection.COS))
                {
                    operatorStack.push(OperatorSelection.COS);
                    expIndex += OperatorSelection.COS.length() - 1;
                    continue;
                }

                if (source.substring(expIndex, expIndex + OperatorSelection.TAN.length()).equals(OperatorSelection.TAN))
                {
                    operatorStack.push(OperatorSelection.TAN);
                    expIndex += OperatorSelection.TAN.length() - 1;
                    continue;
                }

                if (source.substring(expIndex, expIndex + OperatorSelection.ASIN.length()).equals(OperatorSelection.ASIN))
                {
                    operatorStack.push(OperatorSelection.ASIN);
                    expIndex += OperatorSelection.ASIN.length() - 1;
                    continue;
                }

                if (source.substring(expIndex, expIndex + OperatorSelection.ACOS.length()).equals(OperatorSelection.ACOS))
                {
                    operatorStack.push(OperatorSelection.ACOS);
                    expIndex += OperatorSelection.ACOS.length() - 1;
                    continue;
                }

                if (source.substring(expIndex, expIndex + OperatorSelection.ATAN.length()).equals(OperatorSelection.ATAN))
                {
                    operatorStack.push(OperatorSelection.ATAN);
                    expIndex += OperatorSelection.ATAN.length() - 1;
                    continue;
                }


                if (source.substring(expIndex, expIndex + OperatorSelection.SQRT.length()).equals(OperatorSelection.SQRT))
                {
                    operatorStack.push(OperatorSelection.SQRT);
                    expIndex += OperatorSelection.SQRT.length() - 1;
                    continue;
                }
            }
        }
        return expression;
    }

    private boolean isOperand(String operand) {
        for(char symbol : operand.toCharArray()) {
            if(!Character.isDigit(symbol) && (symbol != DOT.charAt(0))) {
                return false;
            }
        }

        return true;
    }

    private void addOperator(String source) throws Exception {
        Operator operator = OperatorSelection.getOperator(source);

        if (operator instanceof BinaryOperator) {
            expression.addButtonName(((BinaryOperator) operator).getButtonsName());
        } else {
            expression.addButtonName(((UnaryOperator) operator).getButtonsName());
        }
    }

    private void addCloseBracket() throws Exception {
        while (true) {
            if (operatorStack.peek().equals(EXPRESSION_BORDER)) {
                throw new Exception("No matches to closing bracket");
            } else if (operatorStack.peek().equals(ButtonPanel.OPEN)) {
                operatorStack.pop();

                break;
            }

            if (!operatorStack.isEmpty()) {
                try {
                    addOperator(operatorStack.pop());
                } catch (Exception ex) {
                    throw new Exception(ex.getMessage());
                }
            }
        }
    }

    private int addOpenBracket(int beginIndex) {
        operatorStack.push(ButtonPanel.OPEN);
        return beginIndex + ButtonPanel.OPEN.length() - 1;
    }

    private int addPlusMinusOperator(int beginIndex) throws Exception {
        if (beginIndex == 0 || source.substring(beginIndex - 1, beginIndex).equals(ButtonPanel.OPEN)) {
            operatorStack.push(source.substring(beginIndex, beginIndex + 1));
        } else {
            while (true) {
                if (operatorStack.peek().equals(EXPRESSION_BORDER)
                        || operatorStack.peek().equals(ButtonPanel.OPEN)) {

                    operatorStack.push(source.substring(beginIndex, beginIndex + 1));

                    break;
                } else {
                    if (!operatorStack.isEmpty()) {
                        try {
                            addOperator(operatorStack.pop());
                        } catch (Exception ex) {
                            throw new Exception(ex.getMessage());
                        }
                    }
                }
            };
        }

        return beginIndex;
    }

    private void endParse() throws Exception {
        while (true) {
            if (operatorStack.peek().equals(EXPRESSION_BORDER)) {
                break;
            } else if (operatorStack.peek().equals(ButtonPanel.OPEN)) {
                throw new Exception("Open bracket in the end of the expression");
            }

            if (!operatorStack.isEmpty()) {
                try {
                    addOperator(operatorStack.pop());
                } catch (Exception ex) {
                    throw new Exception(ex.getMessage());
                }
            }
        }
    }

    private void addMultDivModOperator(int beginIndex) throws Exception {
        while (true) {
            if (operatorStack.peek().equals(OperatorSelection.MULTIPLICATION) ||
                    operatorStack.peek().equals(OperatorSelection.DIVISION) ||
                    operatorStack.peek().equals(OperatorSelection.SQRT) ||
                    operatorStack.peek().equals(OperatorSelection.MOD)) {

                if (!operatorStack.isEmpty()) {
                    try {
                        addOperator(operatorStack.pop());
                    } catch (Exception ex) {
                        throw new Exception(ex.getMessage());
                    }
                }
            } else {
                operatorStack.push(source.substring(beginIndex, beginIndex + 1));
                break;
            }
        }
    }
}
