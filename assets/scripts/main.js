document.addEventListener("DOMContentLoaded", function () {
    // Gọi các hàm cập nhật dữ liệu từ Adafruit feeds và hiển thị lên trang web
    updateTime();
    updateWeatherData();
    
    // Cập nhật thời gian mỗi giây
    setInterval(updateTime, 1000);

    // Cập nhật dữ liệu và biểu đồ mỗi 5 giây
    setInterval(function () {
        updateWeatherData();
        
    }, 5000);
});


// Hàm cập nhật thời gian
function updateTime() {
    const timeElement = document.getElementById("time");
    const now = new Date();
    
    // Sử dụng toLocaleTimeString và toLocaleDateString để lấy thông tin về thời gian và ngày
    const timeString = now.toLocaleTimeString();
    const dateString = now.toLocaleDateString();
    
    // Hiển thị thông tin thời gian và ngày
    timeElement.innerText = `Thời gian hiện tại: ${timeString} ngày ${dateString}`;
}

// Hàm cập nhật dữ liệu thời tiết từ Adafruit feeds
function updateWeatherData() {
    const temperatureElement = document.getElementById("temperature");
    const humidityElement = document.getElementById("humidity");
    const predictElement = document.getElementById("predict");
    const suggestElement = document.getElementById("suggest");

    const temperatureFeed = "temperature";
    const humidityFeed = "humidity";
    const predictFeed = "predict";
    const suggestFeed = "suggest";
    
    const apiKey = "aio_lXyu733ji4SliWqfYRJbBvJpcA2M";  // Thay YOUR_ADAFRUIT_IO_API_KEY bằng API key của bạn
    
    // Gửi yêu cầu cho feed nhiệt độ
    fetch(`https://io.adafruit.com/api/v2/nghiemhunter1904/feeds/${temperatureFeed}/data/last?X-AIO-Key=${apiKey}`)
        .then(response => response.json())
        .then(data => {
            temperatureElement.innerText = `Nhiệt độ hiện tại: ${data.value} °C`;
        });
    
    // Gửi yêu cầu cho feed độ ẩm
    fetch(`https://io.adafruit.com/api/v2/nghiemhunter1904/feeds/${humidityFeed}/data/last?X-AIO-Key=${apiKey}`)
        .then(response => response.json())
        .then(data => {
            humidityElement.innerText = `Độ ẩm hiện tại: ${data.value} %`;
        });
    
    // Gửi yêu cầu cho feed dự đoán thời tiết
    fetch(`https://io.adafruit.com/api/v2/nghiemhunter1904/feeds/${predictFeed}/data/last?X-AIO-Key=${apiKey}`)
        .then(response => response.json())
        .then(data => {
            predictElement.innerText = `Dự đoán thời tiết: ${data.value}`;
            // Xử lý dữ liệu dự đoán thời tiết
        });
    
    // Gửi yêu cầu cho feed gợi ý hành động
    fetch(`https://io.adafruit.com/api/v2/nghiemhunter1904/feeds/${suggestFeed}/data/last?X-AIO-Key=${apiKey}`)
        .then(response => response.json())
        .then(data => {
            suggestElement.innerText = `Gợi ý hành động: ${data.value}`;
        });
    
}
