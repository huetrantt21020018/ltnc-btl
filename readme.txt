Về game
- Nhấn enter để bắt đầu game
- Nhân vật game có thể di chuyển sang trái, sang phải, nhảy lên và rơi xuống nếu không có tấm ván đỡ nó
- Nhảy lên các tấm ván thường để tiến lên, các tấm ván này có thể di chuyển trong một vài cửa.
- Nếu chạm vào các tấm ván có gai hoặc rơi xuống đáy màn hình thì sẽ thua.
- Game có 3 cửa (sáng - trưa - tối), nếu chạm vào đích sẽ qua cửa, qua hết 3 cửa là thắng.
- Cố gắng thu được càng nhiều đồng xu càng tốt, điểm được tính là số đồng xu thu được.
- File ranking xếp hạng 5 điểm cao nhất chỉ khi thắng game.
- Khi thắng game, nếu bạn phá kỉ lục sẽ được thông báo "New highest score", nếu không sẽ thông báo kỉ lục trước đó.
- Game có âm thanh đơn giản.

Về code
- SDL_utils : viết các hàm chuẩn bị SDL, báo lỗi, thu hồi, load ảnh. Nguồn code: slide cô Châu
- SDL_text : chuẩn bị cho việc in chữ ra màn hình, phát hiệu ứng âm thanh. Nguồn lazyfoo, có sửa một vài chi tiết cho phù hợp.
- common : khai báo các biến toàn cục, sử dụng trong toàn bộ dự án
- plat : viết các cấu trúc class về các loại tấm ván, đồng xu, đích.
- player : viết các hàm xử lý hành động người chơi, tính điểm, thua/thắng game
- mainGame : các hàm chuẩn bị map, chuyển cửa, in ra màn hình, sắp xếp thứ hạng
- main : chuẩn bị SDL, vòng lặp game, thu hồi bộ nhớ

Tài nguyên
- Sử dụng SDL, SDL_image, SDL_ttf, SDL_mixer.
- Phông chữ Aller.
- Map được đọc vào từ file.
- Ảnh làm background, ảnh chim cánh cụt, các loại vật phẩm và biển báo được một người bạn em design hộ.
- Âm thanh nền, hiệu ứng thắng thua, nhặt vàng hay nhảy lên: mixkit.co
 
Quá trình thực hiện
first commit: Gồm các hàm cơ bản để sử  dụng SDL.
	 Đã viết thành công một khối hộp chạy từ đầu đến cuối màn hình
second commit: thêm file readme

third commit: Khối hộp nhận điều khiển từ bàn phím có thể đổi hướng
fourth commit: Tách file lần đầu. Thêm file git.ignore
		Cách di chuyển của khối hộp có dạng ném thẳng đứng lên trên từ mặt đất: Nhảy lên chậm dần và rơi xuống nhanh dần

fifth commit: Viết thêm các tấm ván và thuật toán để có thể di chuyển lên trên. Tuy nhiên gặp rất nhiều lỗi.
		Sau đó có fix được các lỗi về di chuyển, tuy nhiên việc random tọa độ các tấm ván không đảm bảo khả năng di chuyển giữa chúng.
sixth commit: Thuật toán random hiệu quả hơn, các khối gạch không ở quá xa cũng như không bít đường di chuyển của nhau. Tỉ lệ các map không bị lỗi 9/10. Tuy nhiên trông map không cân đối, các tấm ván tập trung giữa màn hình.
seventh commit: Các cách random không phù hợp. Bắt buộc dùng map, tuy nhiên các map không thể chơi theo kiểu chơi đến thua. Chia game thành các level, mỗi level một map riêng.
		load ảnh background cho game và nhân vật người chơi là một con cánh cụt. Tuy nhiên mỗi vòng lặp game lại load lại ảnh, nên game chỉ chạy được một thời gian rất ngắn là mất background.
		Có viết thêm các tấm ván đỏ là bẫy, chạm trúng là thua. Ngoài ra, các tấm ván có thể di chuyển.

eighth commit: Các ảnh chỉ load một lần, game chạy ổn định hơn.
		Load thêm ảnh cho các tấm ván bẫy, qua màn, bắt đầu và kết thúc game. Thêm cách tính điểm bằng xu.

nineth commit: Thêm file SDL_text để in chữ ra màn hình. In thành công điểm của người chơi.
		Gói các ảnh lại trong các thư mục

tenth commit: Thêm file ranking và in ra điểm cao nhất.

eleventh commit: Giải phóng bộ nhớ sau khi chạy xong. 
		Thêm hiệu ứng âm thanh.

twelfth commit: Tách file lần 2. dọn dẹp code

final commit: Thêm comment, readme. 






