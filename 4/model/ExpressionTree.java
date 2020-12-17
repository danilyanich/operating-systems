package model;

import view.ButtonPanel;
import java.util.*;


public class ExpressionTree {
    private ExpressionTreeNode root;


    public ExpressionTree() {
        root = null;
    }

    public ExpressionTree(Expression expression) throws Exception {
        root = construct(expression);
    }

    public ExpressionTreeNode getRoot() {
        return root;
    }

    public Expression toInfix() {
        Expression infix = new Expression();
        traverseInfix(root, infix);

        List<ButtonsName> toRemove = new ArrayList<>();

        for (int tokenIterator = 2; tokenIterator < infix.tokens().size(); tokenIterator++) {
            if (isOperand(infix.tokens().get(tokenIterator).source())) {
                if (isBracket(infix.tokens().get(tokenIterator + 1).source())
                        && !(isUnaryOperator(infix.tokens().get(tokenIterator - 2).source()))) {
                    toRemove.add(infix.tokens().get(tokenIterator + 1));
                    toRemove.add(infix.tokens().get(tokenIterator - 1));
                }
            }
        }

        toRemove.add(infix.tokens().get(0));
        toRemove.add(infix.tokens().get(infix.tokens().size() - 1));

        infix.tokens().removeAll(toRemove);

        return infix;
    }

    /*
     *      Util
     */

    private void traverseInfix(ExpressionTreeNode root, Expression expression) {
        expression.addButtonName(new ButtonsName(ButtonPanel.OPEN));

        if (root.getState().equals(ExpressionTreeNode.State.VALUE)) {
            expression.addButtonName(root.getValue().buttonsName());
        }

        if (root.getState().equals(ExpressionTreeNode.State.OPERATOR)) {
            if (root.getRightOperand() != null) {
                traverseInfix(root.getRightOperand(), expression);
            }

            Operator operator = root.getOperator();

            if (operator instanceof BinaryOperator) {
                expression.addButtonName(((BinaryOperator) operator).getButtonsName());
            } else {
                expression.addButtonName(((UnaryOperator) operator).getButtonsName());
            }

            if (root.getLeftOperand() != null) {
                traverseInfix(root.getLeftOperand(), expression);
            }
        }

        expression.addButtonName(new ButtonsName(ButtonPanel.CLOSE));
    }

    private ExpressionTreeNode construct(Expression rpn) throws Exception {
        Deque<ExpressionTreeNode> tokenDeque = new ArrayDeque<>();
        ExpressionTreeNode anyRoot, leftOperand, rightOperand;

        for (ButtonsName token : rpn.tokens()) {
            if (isOperand(token.source())) {
                anyRoot = new ExpressionTreeNode(new Operand(token.source()));
                tokenDeque.push(anyRoot);
            }

            if (isUnaryOperator(token.source()) ||  isBinaryOperator(token.source())) {
                anyRoot = new ExpressionTreeNode(OperatorSelection.getOperator(token.source()));

                leftOperand = tokenDeque.pop();

                if (isBinaryOperator(token.source())) {
                    rightOperand = tokenDeque.pop();
                } else {
                    rightOperand = null;
                }

                anyRoot.setLeftOperand(leftOperand);
                anyRoot.setRightOperand(rightOperand);

                tokenDeque.push(anyRoot);
            }
        }

        anyRoot = tokenDeque.pop();

        return anyRoot;
    }

    private boolean isOperand(String operand) {
        for(char symbol : operand.toCharArray()) {
            if(!Character.isDigit(symbol) && (symbol != '.')) {
                return false;
            }
        }

        return true;
    }

    private boolean isUnaryOperator(String source) {
        try {
            return OperatorSelection.getOperator(source) instanceof UnaryOperator;
        } catch (Exception ex) {
            return false;
        }
    }

    private boolean isBinaryOperator(String source) {
        try {
            return OperatorSelection.getOperator(source) instanceof BinaryOperator;
        } catch (Exception ex) {
            return false;
        }
    }

    private boolean isBracket(String source) {
        return source.equals(ButtonPanel.CLOSE)
                || source.equals(ButtonPanel.OPEN);
    }
}
