package inter;

import lexer.Word;
import symbols.Type;

public class Id extends Expr {
    public int offset; //标识符的相对地址
    public int key;
    static int now;

    public Id(Word id, Type p, int b) {
        super(id, p);
        offset = b;
        key = now++;
    }

    public String toString() {
        return ((Word) op).lexeme + "(" + key + ")";
    }
}
