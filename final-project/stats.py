import csv
import matplotlib.pyplot as plt
import statistics

msgs = {}
senders = {"set"}
messages = {}
msgs_by_sender = {}
timestamps_by_sender = {}


def read_csv(filename):
    with open(filename) as csv_file:
        csv_reader = csv.DictReader(csv_file, delimiter=',')
        for row in csv_reader:
            senders.add(row["sender"])
            add_msg(row)
            add_msgs_by_sender(row)
            add_timestamps_by_sender(row)

def add_msg(row):
    msg = row['sender'] + '\t' + row['msg']
    if msg not in messages:
        messages[msg] = 1
    else:
        messages[msg] += 1


def add_timestamps_by_sender(row):
    sndr = row['sender']
    if (sndr not in timestamps_by_sender):
        timestamps_by_sender[sndr] = [int(row['timestamp'])]
    else:
        timestamps_by_sender[sndr].append(int(row['timestamp']))

def add_msgs_by_sender(row):
    sndr = row['sender']
    if (sndr not in msgs_by_sender):
        msgs_by_sender[sndr] = [row['msg']]
    else:
        msgs_by_sender[sndr].append(row['msg'])

def plot_timestamps():
    for s in senders:
        plt.scatter(
            timestamps_by_sender[s], 
            [int(s)]*len(timestamps_by_sender[s])
        )
    plt.ylabel('aem')
    plt.xlabel('timestamp')
    plt.show()

def print_msg_stats():
    global timestamps_by_sender
    for sender in timestamps_by_sender:
        dt = []
        timestamps_by_sender[sender].sort()
        for i in range (1, len(timestamps_by_sender[sender]) - 1):
            dt.append(timestamps_by_sender[sender][i] - timestamps_by_sender[sender][i-1])
        max_dt = max(dt)
        mean_dt = statistics.mean(dt)
        min_dt = min(dt)
        median_dt = statistics.median(dt)
        print(f"Sender:{sender}\n\tCount:{len(timestamps_by_sender[sender])}\n\t"
            f"Mean:{mean_dt}\n\tMax:{max_dt}\n\t"
            f"Min:{min_dt}\n\tMedian:{median_dt}")


if __name__=="__main__":
    read_csv("stats.csv")
    senders.remove("set")
    plot_timestamps()
    print_msg_stats()
    for key,value in messages.items():
        print(f"{value}\t{key}\t")
