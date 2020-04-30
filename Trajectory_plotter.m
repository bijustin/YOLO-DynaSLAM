%% load data

% ground truth
% change dir
%fileID = fopen('groundtruth_halfsphere.txt','r');
fileID = fopen('groundtruth_xyz.txt','r');
formatSpec = '%f %f %f %f %f %f %f %f';
sizeA = [8 Inf];
traj_gt = fscanf(fileID,formatSpec,sizeA);
fclose(fileID);
traj_gt = traj_gt';
traj_gt(:, 2:4) = traj_gt(:, 2:4) - traj_gt(1, 2:4);

% orb_slam
%fileID = fopen('KeyFrameTrajectory_halfsphere.txt','r');
fileID = fopen('KeyFrameTrajectory_rgbd_xyz3.txt','r');
formatSpec = '%f %f %f %f %f %f %f %f';
sizeA = [8 Inf];
traj = fscanf(fileID,formatSpec,sizeA);
fclose(fileID);
traj = traj';

% dynaslam with yolo
%fileID = fopen('KeyFrameTrajectory_halfsphere_yolo.txt','r');
fileID = fopen('KeyFrameTrajectory_rgbd_xyz_yolo.txt','r');
formatSpec = '%f %f %f %f %f %f %f %f';
sizeA = [8 Inf];
traj_yolo = fscanf(fileID,formatSpec,sizeA);
fclose(fileID);
traj_yolo = traj_yolo';

%% plot
%%{
figure(1);
plot3(traj_gt(:,2), traj_gt(:,3), traj_gt(:,4), 'linewidth', 1.5);
hold on;
plot3(traj(:,2), traj(:,3), traj(:,4), 'linewidth', 1.5);
hold on;
plot3(traj_yolo(:,2), traj_yolo(:,3), traj_yolo(:,4), 'linewidth', 1.5);
axis auto, grid on
xlabel('$X$', 'Interpreter', 'latex')
ylabel('$Y$', 'Interpreter', 'latex')
zlabel('$Z$', 'Interpreter', 'latex')
legend('$Ground\ truth$', '$ORB\_SLAM$', '$YOLO\ Dynamic\ ORB\_SLAM$', 'Interpreter', 'latex', 'location', 'best')
view(-50,34)
%%}

% if use sensor fusion toolbox
%{
tp = theaterPlot('XLimit',[-2 2],'YLimit',[-2 2],'ZLimit',[-0.5 2]);
op = orientationPlotter(tp,'DisplayName','Fused Data',...
    'LocalAxesLength',2);
for i=1:size(traj_gt, 1)
    %plotOrientation(op, quaternion(traj_yolo(i,5),traj_yolo(i,6),traj_yolo(i,7),traj_yolo(i,8)));
    plotOrientation(op, quaternion(traj_gt(i,5),traj_gt(i,6),traj_gt(i,7),traj_gt(i,8)), [traj_gt(i,2) traj_gt(i,3) traj_gt(i,4)]);
    drawnow
    %pause(0.1);
end
%}