fsamp = 1000;

% [h{1}, n{1}] = low_pass_filter(fsamp, [40 60], [0.1 0.1]);
% % figure('Name','Low Pass Filter');
% % freqz(h{1}, 1, 1024, fsamp);
% disp("Low Pass Filter")
% fprintf("M [%d]\n",  n{1});
% coefs{1} = regexprep(num2str(h{1}),'\s+',',');
% fprintf("coefs = {%s};\n", coefs{1});

[h{2}, n{2}] = high_pass_filter(fsamp, [65 90], [0.1 0.1]);
% figure('Name','High Pass Filter');
% freqz(h{2}, 1, 1024, fsamp)
disp("High Pass Filter")
fprintf("M [%d]\n",  n{2});
coefs{2} = regexprep(num2str(h{2}),'\s+',',');
fprintf("coefs = {%s};\n", coefs{2});

% [h{3}, n{3}] = band_pass_filter(fsamp, [50 100 150 200], [0.1 0.1 0.1]);
% figure('Name','Band Pass Filter');
% freqz(h{3}, 1, 1024, fsamp)
% disp("Band Pass Filter")
% fprintf("M [%d]\n",  n{3});
% coefs{3} = regexprep(num2str(h{3}),'\s+',',');
% fprintf("coefs = {%s};\n", coefs{3});


% % y[n] = b0x[n]+b1x[n-1] +...+ bMx[n-M]
% x = sin(100);
% k=0;
% % y(n)=0;
% 
% for i=1:(n{2}+1)
%     y(i) = h{2}(k) * x(i - k); 
%     k=k+1;
% end

