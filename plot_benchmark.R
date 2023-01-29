library(ggplot2)

runtime_k0 <- c(196.412,44.783,22.650,18.661)
runtime_k1 <- c(223.934, 43.806, 21.871, 19.880)
runtime_k2 <- c(230.270, 46.176, 20.92, 20.2850)
q_length <- c(40,60,80,100)
length_40 <-c(196.412, 223.934,230.270)
length_60 <- c(44.783, 43.806, 46.176)
length_80 <- c(22.650, 21.871, 20.92)
length_100 <- c(18.661, 19.880, 20.2850)
k <- c(0,1,2)

plot(q_length,runtime_k0,type='l',col='red', main = ""
     , ylab = "runtime in s", xlab = "length of queries")
lines(q_length,runtime_k1,col='green')
lines(q_length,runtime_k2,col='blue')
legend('topright',c("k0","k1","k2"),lty=1,col=c("red","green","blue"))


plot(k, length_40,
     main="",
     ylab="runtime in s",
     xlab = "number of errors",
     type="l",
     col="blue",ylim=c(0,240),xlim = c(0,3))
lines(k,length_60, col="red")
lines(k,length_80, col="green")
lines(k,length_100, col="violet")
legend("bottomright",
       c("40","60","80","100"),
       fill=c("blue","red","green","violet")
)

#par(mfrow = c(2,2))
par(mar = c(1,1,1,1))
plot(k, length_40,
     main="",
     ylab="runtime in s",
     xlab = "number of errors",
     type="l",
     col="blue")
plot(k, length_60,
     main="",
     ylab="runtime in s",
     xlab = "number of errors",
     type="l",
     col="red")
plot(k, length_80,
     main="",
     ylab="runtime in s",
     xlab = "number of errors",
     type="l",
     col="green")
plot(k, length_100,
     main="",
     ylab="runtime in s",
     xlab = "number of errors",
     type="l",
     col="violet")
legend("bottomright",
       c("40","60","80","100"),
       fill=c("blue","red","green","violet"))


#, 51.87
time_sa_40 <- c(37.06, 38.12, 46.16)
time_fm_40 <- c(0.43, 1.56, 9.74)
number_queries <- c(1000,10000,100000)
number_queries_2 <- c(a,b,c)
#par(mar = 1)
plot(number_queries,time_sa_40, type='l',col='red', main = ""
     , ylab = "time in s", xlab = "number of queries",ylim=c(0,55),xlim = c(1000,102000))
lines(number_queries,time_fm_40,col='green')
legend("topright",inset=c(-0.15,0), bty='n', xpd=NA,
       c("sa", "fm"), fill=c("red","green"))



length_sa <-c(40,60,80,100)
value_sa <- c(37, 32.18, 36.93,37.09)
plot(length_sa,value_sa, type='l',col='red', main = ""
     , ylab = "time in s", xlab = "length of queries")

                

                
                


