package view;

import controller.ExpressionTreeController;
import javafx.geometry.Pos;
import javafx.scene.layout.GridPane;


public class CalculatorForm {
    private TreePanel treePanel;
    private RowPanel rowPanel;
    private ButtonPanel buttonPanel;

    private GridPane gridPane;


    public CalculatorForm(ExpressionTreeController expressionTreeController) {
        rowPanel = new RowPanel();
        treePanel = new TreePanel(rowPanel, expressionTreeController);
        buttonPanel = new ButtonPanel(rowPanel, treePanel, expressionTreeController);

        gridPane = new GridPane();
        configureGridPane();
    }

    public GridPane getGridPane() {
        return gridPane;
    }



    private void configureGridPane() {
        gridPane.setAlignment(Pos.CENTER);

        gridPane.add(treePanel.getGridPane(), 0, 0, 1, 2);
        gridPane.add(rowPanel.getExpRowScrollPane(), 1, 0);
        gridPane.add(buttonPanel.getGridPane(), 1, 1);
    }
}
