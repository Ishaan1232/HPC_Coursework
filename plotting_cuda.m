% Load and parse data from cuda_results.txt
filename = 'cuda_results.txt';
fid = fopen(filename, 'r');

N = [];
SerialTime = [];
CUDATime = [];

% Read the file line by line
while ~feof(fid)
    line = fgetl(fid);
    if contains(line, 'Serial:')
        data = sscanf(line, 'Serial: %d 0m%fs');
        N = [N; data(1)];
        SerialTime = [SerialTime; data(2)];
    elseif contains(line, 'CUDA:')
        data = sscanf(line, 'CUDA: %d 0m%fs');
        CUDATime = [CUDATime; data(2)];
    end
end

fclose(fid);

% Plot the results
figure;
plot(N, SerialTime, '-*', 'LineWidth', 2, 'MarkerSize', 8);
hold on;
plot(N, CUDATime, '-+', 'LineWidth', 2, 'MarkerSize', 8);
hold off;

% Customize the plot
xlabel('Number of Particles ($N$)', Interpreter='latex');
ylabel('Run-time (s)', Interpreter='latex');
legend('Serial', 'CUDA', 'Location', 'NorthWest', Interpreter='latex', fontsize=25);
grid on;
set(gca, 'FontSize', 25);
