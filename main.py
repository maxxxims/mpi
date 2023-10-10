import numpy as np
from matplotlib import pyplot as plt

BYTES_IN_LONG = 8
MCSEC_IN_SEC = 1   # continue in seconds

FONTSIZE = 10
FONTSIZE_TITLE = 18

CLIP_DATA_SIZE = 0.15 * 1e8


def get_latency(path_to_file: str = 'send_nothing/', nodes: int = 1)-> np.float64:
    latencies = []
    times = []
    with open(f'{path_to_file}{nodes}node/data.txt', 'r') as file:
        file.readline()
        for el in file.readlines():
            time, latency = map(float, el.split(" "))
            times.append(time)
            latencies.append(latency * MCSEC_IN_SEC)
    return np.mean(latencies)



def get_data(path_to_file: str = 'send_info/', nodes: int = 1)-> tuple[np.ndarray, np.ndarray]:
    msg_sizes = []
    times = []   
    file_path = f'{path_to_file}{nodes}node/data.txt'
    with open(file_path, 'r') as file:
        file.readline()
        for el in file.readlines():
            msg_size, time, time_norm = map(float, el.split(" "))
            if msg_size * BYTES_IN_LONG < CLIP_DATA_SIZE:
                times.append(time_norm * MCSEC_IN_SEC)
                msg_sizes.append(BYTES_IN_LONG * msg_size)
    return np.array(msg_sizes), np.array(times)


def get_k(msg_sizes: np.ndarray, times: list) -> tuple:
    clip = len(msg_sizes) * 3 // 4
    k, _ = np.polyfit(msg_sizes[clip:], times[clip:], 1)
    return k



def plot_transfer_t(nodes: int = 1, title: str = '', savefig: bool = False):
    latency = get_latency(nodes=nodes)
    x, y = get_data(nodes=nodes)
    k = get_k(x, y)

    print(f'nodes = {nodes}, latency (alpha) = {latency} sec, k (betta) = {k} sec/bytes')
    microsecond_in_seconds = 1e6
    mseconds_in_seconds = 1e3
    coef = microsecond_in_seconds
    plt.plot(x, coef * k * x + latency * coef, label=f'theoretical')
    plt.plot(x, coef * y, label='practical')

    plt.legend()
    plt.grid(alpha=0.6)
    plt.yscale('log')
    plt.xscale('log')
    plt.ylabel('time, microsec', fontsize=FONTSIZE)
    plt.xlabel('msg size, bytes', fontsize=FONTSIZE)
    plt.title(title, fontsize=FONTSIZE_TITLE)
    if savefig:
        plt.savefig(f'{title}.pdf')
    plt.show()

def show_bandwidth(savefig: bool = False):
    bytes_in_mbyes = 1e-6
    coeff = bytes_in_mbyes


    latency1 = get_latency(nodes=1)
    x1, y1 = get_data(nodes=1)
    k1 = get_k(x1, y1)
    plt.plot(x1, coeff * x1 / (latency1 + k1 * x1), label='1 node')

    latency2 = get_latency(nodes=2)
    x2, y2 = get_data(nodes=2)
    k2 = get_k(x2, y2)
    
    plt.plot(x2, coeff * x2 / (latency2 + k2 * x2), label='2 nodes')

    plt.legend()
    plt.grid(alpha=0.6)
    plt.xscale('log')
    plt.yscale('log')
    plt.ylabel('bandwidth, Mbytes / s', fontsize=FONTSIZE)
    plt.xlabel('msg size, bytes', fontsize=FONTSIZE)
    plt.title('bandwidth', fontsize=FONTSIZE_TITLE)
    if savefig:
        plt.savefig('bandwidth.pdf')
    plt.show()



if __name__ == "__main__":
    
    savefig = True
    print('\n \n')
    plot_transfer_t(nodes=1, title='transfer time on 1 node', savefig=savefig)
    plot_transfer_t(nodes=2, title='transfer time on 2 nodes', savefig=savefig)
    show_bandwidth(savefig=savefig)
    print('\n \n')
