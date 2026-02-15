fn main() {
    let mut x = 10;
    let y = 20;

    if x >= y {
        return x + y;
    } else {
        // subtract instead
        let result = x - y;
        return result;
    }

    while x > 0 {
        x = x - 1;
    }

    let name = "hello world";
    let escaped = "line\nnewline";
}
