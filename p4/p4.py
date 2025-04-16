import sys
import copy

class Process:
    def __init__(self, arrival, burst):
        self.arrival_time = arrival
        self.burst_time = burst
        self.remaining_time = burst
        self.start_time = None
        self.finish_time = None

def fcfs(processes):  # First Come First Served
    time = 0
    total_response = total_turnaround = total_wait = 0

    for p in processes:
        if time < p.arrival_time:
            time = p.arrival_time
        p.start_time = time
        p.finish_time = time + p.burst_time
        time = p.finish_time

        response = p.start_time - p.arrival_time
        turnaround = p.finish_time - p.arrival_time
        wait = turnaround - p.burst_time

        total_response += response
        total_turnaround += turnaround
        total_wait += wait

    count = len(processes)
    print("First Come, First Served")
    print(f"Avg. Resp.:{total_response/count:.2f}, Avg. T.A.:{total_turnaround/count:.2f}, Avg. Wait:{total_wait/count:.2f}\n")

def sjf(processes):  # Shortest Job First
    time = 0
    completed = 0
    total_response = total_turnaround = total_wait = 0
    ready = []

    while completed < len(processes):
        for p in processes:
            if p.arrival_time <= time and p.finish_time is None and p not in ready:
                ready.append(p)
        if not ready:
            time += 1
            continue
        ready.sort(key=lambda x: x.burst_time)
        current = ready.pop(0)

        current.start_time = time
        current.finish_time = time + current.burst_time
        time = current.finish_time

        response = current.start_time - current.arrival_time
        turnaround = current.finish_time - current.arrival_time
        wait = turnaround - current.burst_time

        total_response += response
        total_turnaround += turnaround
        total_wait += wait
        completed += 1

    count = len(processes)
    print("Shortest Job First")
    print(f"Avg. Resp.:{total_response/count:.2f}, Avg. T.A.:{total_turnaround/count:.2f}, Avg. Wait:{total_wait/count:.2f}\n")

def srtf(processes):  # Shortest Remaining Time First
    time = 0
    completed = 0
    total_response = total_turnaround = total_wait = 0
    ready = []

    while completed < len(processes):
        for p in processes:
            if p.arrival_time <= time and p.finish_time is None and p not in ready and p.remaining_time > 0:
                ready.append(p)
        if not ready:
            time += 1
            continue
        ready.sort(key=lambda x: x.remaining_time)
        current = ready[0]

        if current.start_time is None:
            current.start_time = time
            total_response += current.start_time - current.arrival_time

        current.remaining_time -= 1
        time += 1

        if current.remaining_time == 0:
            current.finish_time = time
            turnaround = current.finish_time - current.arrival_time
            wait = turnaround - current.burst_time
            total_turnaround += turnaround
            total_wait += wait
            completed += 1
            ready.remove(current)

    count = len(processes)
    print("Shortest Remaining Time First")
    print(f"Avg. Resp.:{total_response/count:.2f}, Avg. T.A.:{total_turnaround/count:.2f}, Avg. Wait:{total_wait/count:.2f}\n")

def rr(processes, quantum=100):  # Round Robin
    time = 0
    completed = 0
    total_response = total_turnaround = total_wait = 0
    queue = []
    arrived = set()

    while completed < len(processes) or queue:
        for i, p in enumerate(processes):
            if p.arrival_time <= time and i not in arrived:
                queue.append(p)
                arrived.add(i)
        if not queue:
            time += 1
            continue

        current = queue.pop(0)

        if current.start_time is None:
            current.start_time = time
            total_response += current.start_time - current.arrival_time

        slice_time = min(quantum, current.remaining_time)
        current.remaining_time -= slice_time
        time += slice_time

        for i, p in enumerate(processes):
            if p.arrival_time <= time and i not in arrived:
                queue.append(p)
                arrived.add(i)

        if current.remaining_time == 0:
            current.finish_time = time
            turnaround = current.finish_time - current.arrival_time
            wait = turnaround - current.burst_time
            total_turnaround += turnaround
            total_wait += wait
            completed += 1
        else:
            queue.append(current)

    count = len(processes)
    print(f"Round Robin with Time Quantum of {quantum}")
    print(f"Avg. Resp.:{total_response/count:.2f}, Avg. T.A.:{total_turnaround/count:.2f}, Avg. Wait:{total_wait/count:.2f}\n")

def main():
    processes = []

    for line in sys.stdin:
        if not line.strip():
            continue
        arrival, burst = map(int, line.strip().split())
        processes.append(Process(arrival, burst))

    quantum = 100
    if len(sys.argv) > 1:
        try:
            quantum = int(sys.argv[1])
        except ValueError:
            pass

    fcfs(copy.deepcopy(processes))
    sjf(copy.deepcopy(processes))
    srtf(copy.deepcopy(processes))
    rr(copy.deepcopy(processes), quantum)

if __name__ == "__main__":
    main()
