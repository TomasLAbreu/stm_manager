function intv = fixed_point(val, sign, word_len, frac_len)
    fiv=fi(val, sign, word_len, frac_len)
    intv=int(fiv);
%     x=(intv*double(2^(-frac_len)));
end