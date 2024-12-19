#pragma once
#include <Siv3D.hpp>

/// @brief 紙吹雪クラス
class Confetti
{
public:

	/// @brief 紙吹雪の一片
	struct Piece
	{
		Vec2 startPos{ 0, 0 };

		Vec2 startVelocity{ 0, 0 };

		double startAngle = 0.0;

		double angularVelocity;

		double size = 24.0;

		double hue = 0.0;

		double time = 0.0;

		[[nodiscard]]
		Vec2 getPos(const Vec2& gravity) const noexcept
		{
			return (startPos + startVelocity * time + 0.5 * gravity * time * time);
		}

		[[nodiscard]]
		double getAngle() const noexcept
		{
			return (startAngle + angularVelocity * time);
		}

		[[nodiscard]]
		ColorF getColor() const noexcept
		{
			return HSV{ hue, 0.85, 0.95 };
		}

		void draw(const Vec2& gravity) const
		{
			RectF{ Arg::center(0, 0), size, (size * 0.8) }
				.scaled(Periodic::Sine1_1(3s, time), 1.0)
				.rotated(getAngle()).movedBy(getPos(gravity)).draw(getColor());
		}

		[[nodiscard]]
		static Piece Make(const Vec2& pos, const Vec2& startVelocity, const double size, const double hue) noexcept
		{
			return
			{
				.startPos = pos,
				.startVelocity = startVelocity,
				.startAngle = Random(2_pi),
				.angularVelocity = (Random(-180_deg, 180_deg) + Random(-180_deg, 180_deg)),
				.size = size,
				.hue = hue,
			};
		}
	};

	/// @brief デフォルトコンストラクタ
	[[nodiscard]]
	Confetti() = default;

	/// @brief 紙吹雪を初期化します
	/// @param lifeTime 紙吹雪の寿命
	/// @param gravity 重力
	[[nodiscard]]
	explicit Confetti(const Duration& lifeTime, const Vec2& gravity = Vec2{ 0, 100 })
		: m_lifeTimeSec{ lifeTime.count() }
		, m_gravity{ gravity } {
	}

	/// @brief 紙吹雪の一片を追加します
	/// @param pos 初期位置
	/// @param startVelocity 初期速度
	/// @param size 紙の大きさ
	/// @param hue 色相
	void add(const Vec2& pos, const Vec2& startVelocity, const double size, const double hue)
	{
		m_pieces << Piece::Make(pos, startVelocity, size, hue);
	}

	/// @brief 紙吹雪をクリアします
	void clear()
	{
		m_pieces.clear();
	}

	/// @brief 紙吹雪を更新します
	/// @param deltaTime 前フレームからの経過時間（秒）
	void update(double deltaTime = Scene::DeltaTime())
	{
		for (auto& piece : m_pieces)
		{
			piece.time += deltaTime;
		}

		m_pieces.remove_if([lifeTime = m_lifeTimeSec](const Piece& piece) { return (lifeTime < piece.time); });
	}

	/// @brief 紙吹雪を描画します
	void draw() const
	{
		for (const auto& piece : m_pieces)
		{
			piece.draw(m_gravity);
		}
	}

	/// @brief 紙吹雪の数を返します
	/// @return 紙吹雪の数
	[[nodiscard]]
	size_t num_pieces() const
	{
		return m_pieces.size();
	}

private:

	Array<Piece> m_pieces;

	double m_lifeTimeSec = 4.0;

	Vec2 m_gravity{ 0, 0 };
};
