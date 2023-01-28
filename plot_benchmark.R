

runtime_k0 <- c(196.412,44.783,22.650,18.661)
runtime_k1 <- c(223.934, 43.806, 21.871, 19.880)
runtime_k2 <- c(230.270, 46.176, 20.92, 20.2850)
q_length <- c(40,60,80,100)

plot(q_length,runtime_k0,type='l',col='red')
lines(q_length,runtime_k1,col='green')
lines(q_length,runtime_k2,col='blue')
legend('topright',c("k0","k1","k2"),lty=1,col=c("red","green","blue"))

