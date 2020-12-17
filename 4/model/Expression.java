package model;

import java.util.ArrayList;
import java.util.List;


public class Expression {
    private List <ButtonsName> buttonsNames;


    public Expression() {
        buttonsNames = new ArrayList<>();
    }

    public List<ButtonsName> tokens() {
        return buttonsNames;
    }

    public void addButtonName(ButtonsName buttonsName) {
        buttonsNames.add(buttonsName);
    }

    @Override
    public String toString() {
        String toString = "";
        for (ButtonsName buttonsName : buttonsNames) {
            toString = toString.concat(buttonsName.toString());
        }

        return toString;
    }
}
