package view;

import javafx.scene.control.ScrollPane;
import javafx.scene.control.TextField;
import javafx.scene.text.Font;


public class RowPanel {
    private TextField expressionRowTextField;

    private ScrollPane expRowScrollPane;


    public RowPanel() {
        expressionRowTextField = new TextField();
        configureExpressionRowTextField();
        expRowScrollPane = new ScrollPane();
        configureExpRowScrollPane();
    }

    public ScrollPane getExpRowScrollPane() {
        return expRowScrollPane;
    }

    public TextField getExpressionRowTextField() {
        return expressionRowTextField;
    }



    private void configureExpressionRowTextField() {

        expressionRowTextField.setFont(new Font(14));
        expressionRowTextField.setPrefHeight(50);
        expressionRowTextField.setPrefWidth(1000);

        expressionRowTextField.textProperty().addListener(e -> {
            expressionRowTextField.positionCaret(expressionRowTextField.getText().length());
        });
    }

    private void configureExpRowScrollPane() {
        expRowScrollPane.setContent(expressionRowTextField);
        expRowScrollPane.setVbarPolicy(ScrollPane.ScrollBarPolicy.NEVER);
        expRowScrollPane.setHbarPolicy(ScrollPane.ScrollBarPolicy.ALWAYS);
        expRowScrollPane.setPrefWidth(100);
    }
}
