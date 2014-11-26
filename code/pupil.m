%**************************************************************************
% Measuring Pupil paramters from images
% Main file
%
% Author: Alankar Kotwal <alankarkotwal13@gmail.com>
%
% Make sure you run this file from the pupil-measurements directory
%**************************************************************************

% Load the image, convert to YCbCr and get luminance
eye = rgb2ycbcr(imread('images/eye.jpg'));
eyeL = eye(:, :, 1);
eyeSize = size(eyeL);

% Do a constrast stretch
eyeLC = histeq(eyeL);

% Threshold now
eyeLCT = 1-im2bw(eyeLC, 0.05);

% Get connected components and pixel list of largest size
c = bwconncomp(eyeLCT);
numPixels = cellfun(@numel, c.PixelIdxList);
[biggest,idx] = max(numPixels);
eyeLCT(c.PixelIdxList{idx}) = 2;
eyeLCT = floor(eyeLCT/2);

% Find circles
s = regionprops(eyeLCT, 'centroid');
cen = round(s.Centroid);
ar = bwarea(eyeLCT);
rad = sqrt(ar/pi);

% Plot
imshow(ycbcr2rgb(eye));
viscircles(cen, rad,'EdgeColor','b');