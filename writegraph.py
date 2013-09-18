from random import randint;

def WriteGraph(filename):
    with open(filename, "w") as f:
        for i in range(1000):
            src = randint(0, 500);
            dst = randint(0, 500);
            w = float(randint(0,1000))/1000;
            f.write( "{} {} {}\n".format(src, dst, w));

def main():
    WriteGraph("graph.txt");
    
if __name__ == "__main__":
    main();
