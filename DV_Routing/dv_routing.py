import sys

INF = float('inf')


def read_token(prompt=''):
    line = input(prompt).strip()
    while not line:
        line = input().strip()
    return line


def parse_input():
    N = int(read_token())
    matrix = []
    for i in range(N):
        row = []
        for j in range(N):
            token = read_token()
            row.append(INF if token == 'f' else int(token))
        matrix.append(row)
    return N, matrix


def bellman_ford(source, N, matrix):
    dist = [INF] * N
    dist[source] = 0

    for _ in range(N - 1):
        for u in range(N):
            if dist[u] == INF:
                continue
            for v in range(N):
                if matrix[u][v] != INF:
                    if dist[u] + matrix[u][v] < dist[v]:
                        dist[v] = dist[u] + matrix[u][v]

    # Nth iteration: check for negative cycle
    for u in range(N):
        if dist[u] == INF:
            continue
        for v in range(N):
            if matrix[u][v] != INF:
                if dist[u] + matrix[u][v] < dist[v]:
                    return None  # negative cycle reachable from source

    return dist


def format_dist(dist, N):
    if dist is None:
        return '[' + ', '.join(['None'] * N) + ']'
    parts = []
    for d in dist:
        parts.append('inf' if d == INF else str(d))
    return '[' + ', '.join(parts) + ']'


def main():
    N, matrix = parse_input()
    for s in range(N):
        dist = bellman_ford(s, N, matrix)
        print(f"Node {s}: {format_dist(dist, N)}")


if __name__ == '__main__':
    main()
