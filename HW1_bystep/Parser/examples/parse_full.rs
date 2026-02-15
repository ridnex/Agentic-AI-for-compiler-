fn main() {
    let x = 10;
    let mut y = 0;

    while x > 0 {
        if x > 5 {
            let a = x + 1;
        } else {
            let b = x - 1;
        }
        let x = x - 1;
    }

    return y;
}
