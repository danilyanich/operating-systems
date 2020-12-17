package view;

import controller.ExpressionTreeController;
import javafx.scene.control.TextField;
import javafx.scene.control.TreeItem;
import javafx.scene.control.TreeView;
import javafx.scene.layout.GridPane;
import javafx.scene.text.Font;
import model.*;


public class TreePanel {
    private static final int PANEL_WIDTH = 200;
    private ExpressionTreeController expressionTreeController;
    private TreeView<ExpressionTreeNode> expressionTreeView;
    private RowPanel expressionRowPanel;
    private TextField result;
    private GridPane gridPane;


    public TreePanel(RowPanel expressionRowPanel, ExpressionTreeController expressionTreeController)
    {
        this.expressionTreeController = expressionTreeController;
        expressionTreeView = new TreeView<>();
        this.expressionRowPanel = expressionRowPanel;
        gridPane = new GridPane();
        configureGridPane();
    }

    public GridPane getGridPane() {
        return gridPane;
    }


    private void configureGridPane()
    {
        result = new TextField();
        result.setEditable(false);
        result.setFocusTraversable(false);
        result.setFont(new Font(18));
        result.setPrefHeight(50);
        result.setPrefWidth(PANEL_WIDTH);
        expressionTreeView.setEditable(false);
        expressionTreeView.setFocusTraversable(false);
        expressionTreeView.setPrefHeight(315);
        expressionTreeView.setPrefWidth(PANEL_WIDTH);
        gridPane.add(result, 0, 0);
        gridPane.add(expressionTreeView, 0, 1);
    }

    public void construct()
    {
        expressionTreeView.setRoot(new TreeItem<>(expressionTreeController.getRoot()));
        recursiveInitialize(expressionTreeController.getRoot(), expressionTreeView.getRoot());
        traverseExpand(expressionTreeView.getRoot());
        result.setText(expressionTreeController.result().getSource());
    }

    private void recursiveInitialize(ExpressionTreeNode expressionTreeNode, TreeItem<ExpressionTreeNode> treeItem)
    {
        treeItem.expandedProperty().addListener(e -> {
            treeItem.getValue().setState(treeItem.isExpanded() ?
                    ExpressionTreeNode.State.OPERATOR :
                    ExpressionTreeNode.State.VALUE);

            expressionRowPanel.getExpressionRowTextField().setText(
                    expressionTreeController.infix().toString()
            );
        });

        if (expressionTreeNode.getRightOperand() != null) {
            TreeItem<ExpressionTreeNode> right = new TreeItem<>(expressionTreeNode.getRightOperand());

            treeItem.getChildren().add(right);
            recursiveInitialize(expressionTreeNode.getRightOperand(), right);
        }

        if (expressionTreeNode.getLeftOperand() != null) {
            TreeItem<ExpressionTreeNode> left = new TreeItem<>(expressionTreeNode.getLeftOperand());

            treeItem.getChildren().add(left);
            recursiveInitialize(expressionTreeNode.getLeftOperand(), left);
        }
    }

    private void traverseExpand(TreeItem<ExpressionTreeNode> root) {
        for (TreeItem<ExpressionTreeNode> child : root.getChildren()) {
            traverseExpand(child);
            root.setExpanded(true);
        }
    }
}
