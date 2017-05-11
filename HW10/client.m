clear;
close all;
clc;
if ~isempty(instrfind)
	fclose(instrfind);
	delete(instrfind);
end
s=serial('COM3');
fopen(s);
fprintf(s,'%c\n','r');
n=fscanf(s,'%d');
data=[];
for i=1:n
	data=[data;fscanf(s,'%f %f %f %f')'];
end
figure;
hold on;
plot(data(:,1));
plot(data(:,2));
plot(data(:,3));
plot(data(:,4));
legend('raw','MAF','IIR','FIR');
ylabel('z acceleration');
xlabel('#sample');