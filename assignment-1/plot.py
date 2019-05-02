#!/usr/bin/env python

import csv
import matplotlib.pyplot as plt
import numpy as np

x = []
y = []
z = []

def read_csv(filename):
    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        count = 0
        for row in csv_reader:
            if (count == 0):
                count += 1
            else:
                x.append(int(row[0]))
                y.append(float(row[1]))

def get_stats():
    max_ = max(y)
    min_ = min(y)
    mean_ = np.mean(np.array(y))
    std_ = np.std(np.array(y))
    median_ = np.median(np.array(y))
    print("max:{}\tmin:{}\tmean:{}\tstd:{}\tmedian:{}"
    .format(max_, min_, mean_, std_, median_))


def make_plot(id):
#    plt.plot(x, y, 'ro', x, y)
    plt.subplot(id)
    plt.plot(x, y)
    plt.plot(x, [100000]*len(x), 'r')
    plt.ylabel('duration (usec)')
    plt.xlabel('sample')
    plt.title('Sampling interval duration')

if __name__ == "__main__":
    try:
        read_csv("./log/sampling_a.csv")
        get_stats()
        make_plot(211)
        read_csv("./log/sampling_b.csv")
        get_stats()
        make_plot(212)
        plt.show()
    except KeyboardInterrupt:
        print("exiting...")
