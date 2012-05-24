OBJS = common/except/BasicExcept.o \
	captcha/Captcha.o
CXXFLAGS = -g -O3 -flto

all: train recog

train: train.o $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

recog: recog.o $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c -I. $< -W -Wall -Werror -Wextra $(CXXFLAGS)

clean:
	rm -f $(OBJS) train recog *.o
