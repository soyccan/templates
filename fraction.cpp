struct Frac { // fraction
    int num; // numerator
    int den; // denominator
    Frac(int num,int den): num(num),den(den) {}
    Frac simplify() {
        int gcd = __gcd(num,den);
        num/=gcd;
        den/=gcd;
        return *this;
    }
};
Frac operator+(Frac a,Frac b) {
    return Frac(a.num*b.den + b.num*a.den, a.den*b.den).simplify();
}
Frac operator-(Frac a,Frac b) {
    return Frac(a.num*b.den - b.num*a.den, a.den*b.den).simplify();
}
Frac operator*(Frac a,Frac b) {
    return Frac(a.num*b.num, a.den*b.den).simplify();
}
Frac tofrac(string s) {
    int i=0, a=0, b=0;
    for (; s[i]!='/' && i<s.size(); i++)
        a=a*10+s[i]-'0';
    i++;
    for (; i<s.size(); i++)
        b=b*10+s[i]-'0';
    if (b==0) b=1;
    return Frac(a,b);
}
