package controller;

import model.*;


public class ExpressionTreeController {
    private ExpressionTree expressionTree;
    private Expression expressionRPN;


    public ExpressionTreeController(ExpressionTree expressionTree) {
        this.expressionTree = expressionTree;
    }

    public void createTree(String expression) throws Exception {
        try {
            expressionRPN = new ExpressionTransformer(expression).convert();
        } catch (Exception ex) {
            throw  new Exception("Cannot parse expression due to next errors: " + ex.getMessage());
        }

        try {
            expressionTree = new ExpressionTree(expressionRPN);
        } catch (Exception ex) {
            throw  new Exception("Cannot parse expression due to next errors: " + ex.getMessage());
        }
    }

    public ExpressionTreeNode getRoot() {
        return expressionTree.getRoot();
    }

    public Operand result() {
        return expressionTree.getRoot().getValue();
    }

    public Expression infix() {
        return expressionTree.toInfix();
    }
}
